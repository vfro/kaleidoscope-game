#include "StdAfx.h"

#include <string>
#include <fstream>
#include <cassert>
#include <algorithm>

#include "KDScene.h"
#include "KDFormats.h"

template<typename TSavedType>
void KDSaveToFile/*<TSavedType>*/(std::ostream& os, const TSavedType& value)
{
   const char* savedValue = reinterpret_cast<const char*>(&value);
   os.write(savedValue, sizeof(TSavedType));
}

template<typename TLoadedType>
void KDLoadFromFile/*<TLoadedType>*/(std::istream& is, TLoadedType& value)
{
   char* loadedValue = reinterpret_cast<char*>(&value);
   is.read(loadedValue, sizeof(TLoadedType));
}

void KDSaveObjectList(std::ostream& os, aHexObjectList& hexList)
{
   int border = -1;
   KDSaveToFile(os, border);

   int hexCount = hexList.size();
   KDSaveToFile(os, hexCount);

   for(aHexObjectList::iterator where = hexList.begin();
       where != hexList.end();
       ++where
      )
   {
      CPoint Pos = (*where)->GetPosition();
      KDSaveToFile(os, Pos.x);
      KDSaveToFile(os, Pos.y);

      for (size_t colorNum = 0; colorNum < KDHexFormat::triAmount; ++colorNum)
      {
         int triColor = (*where)->GetColor(colorNum);
         KDSaveToFile(os, triColor);
      }
   }
}

void KDLoadObjectList(std::istream& is,
                      aHexObjectList& hexList,
                      CDC& dcPattern,
                      KDHexFormat::aHexColorParam& colorPattern
                     )
{
   int border = 0;
   KDLoadFromFile(is, border);
   assert(border == -1);

   hexList.clear();
   int hexCount = 0;
   KDLoadFromFile(is, hexCount);

   for(int i = 0; i < hexCount; ++i)
   {
      CPoint hexPos;
      KDLoadFromFile(is, hexPos.x);
      KDLoadFromFile(is, hexPos.y);

      bool grayHexagone = true;
      KDHexFormat::hexArray triColors;
      for (size_t colorNum = 0; colorNum < KDHexFormat::triAmount; ++colorNum)
      {
         KDLoadFromFile(is, triColors[colorNum]);
         if (triColors[colorNum] != 0)
            grayHexagone = false;
      }

      if (grayHexagone)
      {
         aGrayHexagon* newHex = new aGrayHexagon(dcPattern, hexPos, colorPattern);
         hexList.push_back(newHex);
      }
      else
      {
         aHexagon* newHex = new aHexagon(dcPattern, hexPos, colorPattern, triColors);
         hexList.push_back(newHex);
      }
   }
}

aKDScene::aKDScene()
   : sceneBitmap(0),
     param(0),
     moving(false),
     sceneState(false),
     backGround(0),
     hexCount(0),
     winState(false),
     winStatus(false)
{
   objectList.clear();
   movableObjectList.clear();
   rotateObjectList.clear();
   hexInGridList.clear();
}

aKDScene::~aKDScene()
{
   if (backGround != 0)
      delete backGround;
   DestroyObjects();
   DeleteBitmap();
}

void aKDScene::SaveGame(std::ostream& os)
{
   int gameType = param.GetColorsAmount() - KDFormats::minColors;
   KDSaveToFile(os, gameType);

   int scores = gamePlay.GetScore();
   KDSaveToFile(os, scores);

   int trashed = gamePlay.GetTrashed();
   KDSaveToFile(os, trashed);

   KDSaveToFile(os, hexCount);

   int colorsArray = param.GetColorsArray();
   KDSaveToFile(os, colorsArray);

   KDSaveObjectList(os, rotateObjectList);
   KDSaveObjectList(os, hexInGridList);
}

int aKDScene::GetColorSet() const
{
   return param.GetColorsAmount() - KDFormats::minColors - 1;
}

void aKDScene::LoadGame(std::istream& is)
{
   sceneState = true;
   winState = true;
   winStatus = false;

   int gameType = 0;
   KDLoadFromFile(is, gameType);
   int scores = 0;
   KDLoadFromFile(is, scores);
   int trashed = 0;
   KDLoadFromFile(is, trashed);
   KDLoadFromFile(is, hexCount);
   int colorArray = 0;
   KDLoadFromFile(is, colorArray);

   param.CustomizeColors(gameType + KDFormats::minColors, colorArray);

   gamePlay.StartGame();
   gamePlay.SetScore(scores);
   gamePlay.SetTrashed(trashed);

   aHexObjectList sceneObjects;
   KDLoadObjectList(is, sceneObjects, sceneDevice, param);

   objectList.clear();
   movableObjectList.clear();
   rotateObjectList.clear();
   objectList.insert(objectList.begin(), sceneObjects.begin(), sceneObjects.end());
   movableObjectList.insert(movableObjectList.begin(), sceneObjects.begin(), sceneObjects.end());
   rotateObjectList.insert(rotateObjectList.begin(), sceneObjects.begin(), sceneObjects.end());

   sceneObjects.clear();
   KDLoadObjectList(is, sceneObjects, sceneDevice, param);

   hexInGridList.clear();
   objectList.insert(objectList.begin(), sceneObjects.begin(), sceneObjects.end());
   hexInGridList.insert(hexInGridList.begin(), sceneObjects.begin(), sceneObjects.end());

   if (hexCount == backGround->GridSize())
   {
      winState = false;
      winStatus = true;
   }
}

bool aKDScene::loadSave(CWnd* parrent)
{
   bool gameLoaded = false;
   char szFilters[]=
      "Kaleidoscope Saves (*.kds)|*.kds|All Files (*.*)|*.*||";
   char szExtention[] = "kds";

   if (sceneState)
   {
      if (winState == false)
         return false;

      CFileDialog dlg(FALSE, szExtention, 0,
                      OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                      szFilters, parrent
                     );
      if (dlg.DoModal() != IDOK)
         return false;

      std::string fileName = dlg.GetPathName();

      std::string errorString = std::string();

      std::ofstream outFile(fileName.c_str(), std::ios::binary);

      if (!outFile)
      {
         errorString = "Current game cannot be saved to file \"";
         errorString += fileName;
         errorString += "\" because file is not writable.";
      }
      else
      {
         try
         {
            SaveGame(outFile);
         }
         catch(std::exception& exp)
         {
            errorString = exp.what();
         }
      }

      if (errorString != std::string())
      {
         ::MessageBox(0, errorString.c_str(), "Kaleidoscope", MB_ICONWARNING | MB_OK | MB_TASKMODAL);
      }

      outFile.close();
   }
   else
   {
      CFileDialog dlg(TRUE, szExtention, 0,
                      OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
                      szFilters, parrent
                     );
      if (dlg.DoModal() != IDOK)
         return false;

      std::string fileName = dlg.GetPathName();

      std::string errorString = std::string();

      std::ifstream inFile(fileName.c_str(), std::ios::binary);

      if (!inFile)
      {
         errorString = "Game cannot be restored from file \"";
         errorString += fileName;
         errorString += "\" because file is not readable.";
      }
      else
      {
         try
         {
            LoadGame(inFile);
         }
         catch(std::exception& exp)
         {
            errorString = exp.what();
         }
      }

      if (errorString != std::string())
      {
         ::MessageBox(0, errorString.c_str(), "Kaleidoscope", MB_ICONWARNING | MB_OK | MB_TASKMODAL);
      }
      else
      {
         gameLoaded = true;
      }

      inFile.close();
   }
   return gameLoaded;
}

void aKDScene::SelectDC(CDC& dc)
{
   CreateBitmap();

   sceneDevice.CreateCompatibleDC(&dc);
   sceneBitmap->CreateCompatibleBitmap( &dc , 
                                        KDFormats::sceneWidth,
                                        KDFormats::sceneHeight
                                      );

   sceneDevice.SelectObject(sceneBitmap);
   if (backGround == 0)
      backGround = new aKDBackGround(dc);
}

void aKDScene::Draw( CDC& dc)
{
   CreateAppearance();

   dc.BitBlt( KDFormats::sceneLeft, KDFormats::sceneTop,
              KDFormats::sceneWidth, KDFormats::sceneHeight,
              &sceneDevice, 0, 0, SRCCOPY);
}

void aKDScene::GenerateObjects()
{
   DestroyObjects();

   int i;
   int delta = 48;
   if (sceneState)
   {
      for (i = 0; i < 3; i++)
      {
         aHexagon* hex = new aHexagon(sceneDevice, CPoint(20 + i*delta, 340), param);
         AddObject(hex);
         AddMovableObject(hex);
         AddRotateObject(hex);
      }
   }
}

void aKDScene::CreateAppearance()
{
//   if (backGround != 0)
      backGround->Draw(sceneDevice);
   DrawObjects();
   if (sceneState)
      gamePlay.Draw(sceneDevice,param);
}

void aKDScene::CreateBitmap()
{
   if (sceneBitmap != 0)
      delete sceneBitmap;
   sceneBitmap = new CBitmap;
}

void aKDScene::DeleteBitmap()
{
   if (sceneBitmap != 0)
      delete sceneBitmap;
   sceneBitmap = 0;
}

void aKDScene::DestroyObjects()
{
   for (aObjectList::const_iterator i = objectList.begin(); i != objectList.end(); i++)
      delete *i;
   objectList.clear();
   movableObjectList.clear();
   rotateObjectList.clear();
   hexInGridList.clear();
}

void aKDScene::DrawObjects()
{
   for (aObjectList::iterator i = objectList.begin(); i != objectList.end(); i++)
      (*i)->Draw( sceneDevice );
}

void aKDScene::AddObject(aKDSceneObject* obj)
{
   objectList.push_back(obj);
}

void aKDScene::AddMovableObject(aKDSceneObject* obj)
{
   movableObjectList.push_back(obj);
}

void aKDScene::AddRotateObject(aHexagon* obj)
{
   rotateObjectList.push_back(obj);
}

void aKDScene::OnMoveCursor(UINT /*nFlags*/, CPoint point)
{
   CPoint moveTo;
   if (!sceneState)
      return;
   if (!winState)
      return;
   if (moving)
   {
      moveTo = point - movingDelta;
      if (moveTo.x < 0)
         moveTo.x = 0;
      if (moveTo.y < 0)
         moveTo.y = 0;
      if (moveTo.x > KDFormats::sceneWidth - moveObject->GetSize().cx)
         moveTo.x = KDFormats::sceneWidth - moveObject->GetSize().cx;
      if (moveTo.y > KDFormats::sceneHeight - moveObject->GetSize().cy)
         moveTo.y = KDFormats::sceneHeight - moveObject->GetSize().cy;
      moveObject->ReMove(moveTo);
   }
}

void aKDScene::OnRightButtonDown(UINT /*nFlags*/, CPoint point)
{
   if (!sceneState)
      return;
   if (!winState)
      return;
   aHexagon* obj = GetRotatedObject(point);
   if (obj != 0)
      obj->Rotate(true, param);
}

bool aKDScene::WinStatus(int& score)
{
   score = 0;
   if (sceneState)
      score = gamePlay.GetScore();
   return winStatus;
}

void aKDScene::OnLeftButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{
   int coincide;
   if (!sceneState)
      return;
   if (!winState)
      return;
   if (!moving)
      return;
   moving = false;
   CPoint newPlace = 
      backGround->GetPlace(KDFormats::aHexParam::getHexCenter(
            moveObject->GetPosition(),KDFormats::gridHexSize));
   aObjectList::iterator itObj;
   if (newPlace == CPoint(-1, -1))
      newPlace = oldPlace;

   if ((oldPlace != newPlace) && (backGround->IsGrid(newPlace)))
   {
      moveObject->ReMove(newPlace);
      coincide = moveObject->InteractWithGrid(hexInGridList, objectList);
      if ( coincide == -1)
      {
         newPlace = oldPlace;
         moveObject->ReMove(newPlace);
      } else
      {
         hexCount++;
         RemoveMoveableObject(moveObject);
         RemoveRotateObject(moveObject);
         gamePlay.AddHexagon(coincide, param);
         if (hexCount == backGround->GridSize())
         {
            winState = false;
            winStatus = true;
         }
      }
   } else
   if (oldPlace != newPlace)
     for (aObjectList::iterator i = objectList.begin();
          i != objectList.end(); i++)
             if ((moveObject != (*i)) &&
                (newPlace == (*i)->GetPosition()))
                newPlace = oldPlace;


   moveObject->ReMove(newPlace);
   if (backGround->IsTrash(newPlace))
   {
      InTrash(moveObject);
      gamePlay.InTrash(param);
      if (gamePlay.GetScore() < 0)
      {
         winState = false;
      }
   }

   if (newPlace == CPoint(-1, -1))
      InTrash(moveObject);

   if ((sceneState) && 
       (newPlace != oldPlace) && 
       (backGround->IsGet(oldPlace))
      )
   {
      aHexagon* hex; 
      if (rand() % 100 < KDFormats::procentGray)
         hex = new aGrayHexagon(sceneDevice, CPoint(oldPlace), param);
         else
         hex = new aHexagon(sceneDevice, CPoint(oldPlace), param);
      AddObject(hex);
      AddMovableObject(hex);
      AddRotateObject(hex);
   }
}

void aKDScene::OnLeftButtonDown(UINT /*nFlags*/, CPoint point)
{
   aKDSceneObject* obj;
   aObjectList::iterator i;
   if (!sceneState)
      return;
   if (!winState)
      return;
   if (moving)
      return;
   obj = GetMovedObject(point);
   if (obj == 0)
      return;
   moving = true;
   moveObject = obj;
   movingDelta.x = point.x - obj->GetPosition().x;
   movingDelta.y = point.y - obj->GetPosition().y;
   oldPlace = obj->GetPosition();
}

aKDSceneObject* aKDScene::GetMovedObject(CPoint pos)
{
   CPoint objPos;
   CSize objSize;
   aKDSceneObject* obj;
   aObjectList::iterator itObj;
   aHexagon* objRotate;
   aRotateObjectList::iterator itRotate;
   for (aObjectList::iterator i = movableObjectList.begin(); 
        i != movableObjectList.end(); i++
       )
   {
      objPos = (*i)->GetPosition();
      objSize = (*i)->GetSize();
      if (((objPos.x < pos.x ) && (objPos.x + objSize.cx > pos.x)) &&
          ((objPos.y < pos.y ) && (objPos.y + objSize.cy > pos.y)))
      {
         obj = *i;
         movableObjectList.erase(i);
         movableObjectList.push_front(obj);
         itObj = std::find(objectList.begin(), objectList.end(), obj);
         if (*itObj == obj)
         {
            objectList.erase(itObj);
            objectList.push_back(obj);
         }
         itRotate = std::find(rotateObjectList.begin(),
                              rotateObjectList.end(), obj);
         if (dynamic_cast<aKDSceneObject*>(*itRotate) == obj)
         {
            objRotate = (*itRotate);
            rotateObjectList.erase(itRotate);
            rotateObjectList.push_front(objRotate);
         }

         return obj;
      }
   }
   return 0;
}

aHexagon* aKDScene::GetRotatedObject(CPoint pos)
{
   CPoint objPos;
   CSize objSize;
   for (aRotateObjectList::const_iterator i = rotateObjectList.begin(); 
        i != rotateObjectList.end(); i++
       )
   {
      objPos = (*i)->GetPosition();
      objSize = (*i)->GetSize();
      if (((objPos.x < pos.x ) && (objPos.x + objSize.cx > pos.x)) &&
          ((objPos.y < pos.y ) && (objPos.y + objSize.cy > pos.y)))
          return *i;
   }
   return 0;
}

void aKDScene::StartGame(int colorSet)
{
   sceneState = true;
   winState = true;
   winStatus = false;
   hexCount = 0;
   param.CustomizeColors(colorSet + KDFormats::minColors + 1);
   GenerateObjects();
   gamePlay.StartGame();
}

void aKDScene::StopGame()
{
   sceneState = false;
   winState = false;
   winStatus = false;
   moving = false;
   hexCount = 0;
   GenerateObjects();
   gamePlay.StopGame();
}

void aKDScene::InTrash(aKDSceneObject* obj)
{
   RemoveObject(obj);
   delete obj;
}

void aKDScene::RemoveObject(aKDSceneObject* obj)
{
   aObjectList::iterator itObj;
   itObj = std::find(objectList.begin(),
                     objectList.end(),obj
                    );
   if (itObj != objectList.end())
      objectList.erase(itObj);
   RemoveMoveableObject(obj);
   RemoveRotateObject(obj);
}

void aKDScene::RemoveMoveableObject(aKDSceneObject* obj)
{
   aObjectList::iterator itObj;
   itObj = std::find(movableObjectList.begin(),
                     movableObjectList.end(),obj
                    );
   if (itObj != movableObjectList.end())
      movableObjectList.erase(itObj);
}

void aKDScene::RemoveRotateObject(aKDSceneObject* obj)
{
   aRotateObjectList::iterator itRotate;
   itRotate = std::find(rotateObjectList.begin(),
                        rotateObjectList.end(),obj
                       );
   if (itRotate != rotateObjectList.end())
      rotateObjectList.erase(itRotate);
}

aHexagon* aKDScene::findObj(const CPoint& point)
{
   for (aHexObjectList::iterator i = hexInGridList.begin();
        i != hexInGridList.end(); i++
       )
   {
      if ( (*i)->GetPosition() == point)
         return (*i);
   }
   return 0;
}


