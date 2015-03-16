#ifndef KDSCENE
#define KDSCENE

#include <iostream>

#include "KDSceneObj.h"
#include "KDHexFormat.h"
#include "KDGamePlay.h"

class aKDScene
{
public:
   aKDScene();
   ~aKDScene();

   void SelectDC(CDC& dc);
   void GenerateObjects();

   void OnMoveCursor(UINT nFlags, CPoint point);
   void OnLeftButtonUp(UINT nFlags, CPoint point);
   void OnLeftButtonDown(UINT nFlags, CPoint point);
   void OnRightButtonDown(UINT nFlags, CPoint point);

   void Draw(CDC& dc);

   void StartGame(int colorSet);
   void StopGame();
   bool WinStatus(int& score);

   int GetColorSet() const;
   bool loadSave(CWnd* parrent);
private:
   void CreateBitmap();
   void DeleteBitmap();
   void DestroyObjects();

   aHexagon* findObj(const CPoint& point);

   void AddObject(aKDSceneObject* obj);
   void RemoveObject(aKDSceneObject* obj);
   void AddMovableObject(aKDSceneObject* obj);
   void RemoveMoveableObject(aKDSceneObject* obj);
   void AddRotateObject(aHexagon* obj);
   void RemoveRotateObject(aKDSceneObject* obj);

   void InTrash(aKDSceneObject* obj);

   void CreateAppearance();
   void DrawObjects();
   aKDSceneObject* GetMovedObject(CPoint pos);
   aHexagon* GetRotatedObject(CPoint pos);

   void SaveGame(std::ostream& os);
   void LoadGame(std::istream& is);

   CDC sceneDevice;
   CBitmap* sceneBitmap;
   aKDBackGround* backGround;
   bool sceneState;
   bool winState;
   bool winStatus;

   aObjectList objectList;
   aObjectList movableObjectList;
   aRotateObjectList rotateObjectList;
   aHexObjectList hexInGridList;

   KDHexFormat::aHexColorParam param;

   bool moving;
   int hexCount;
   CPoint movingDelta;
   CPoint oldPlace;
   aKDSceneObject* moveObject;

   aGamePlay gamePlay;
};

#endif