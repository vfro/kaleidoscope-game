#include "StdAfx.h"

#include "KDSceneObj.h"
#include "KDFormats.h"

aKDSceneObject::aKDSceneObject(CDC& dc, const CPoint& pos, const CSize& viewSize)
   : size(viewSize),
     position(pos)
{
   view.CreateCompatibleBitmap( &dc, size.cx, size.cy);
   device.CreateCompatibleDC( &dc );
   oldBitmap = device.SelectObject( &view );
}

aKDSceneObject::~aKDSceneObject()
{
   device.SelectObject(oldBitmap);
}

const CPoint& aKDSceneObject::GetPosition() const
{
   return position;
}

const CSize& aKDSceneObject::GetSize() const
{
   return size;
}

void aKDSceneObject::ReMove(const CPoint& pos)
{
   position = pos;
}

void aKDSceneObject::Draw(CDC& dc)
{
   dc.BitBlt(position.x, position.y, size.cx, size.cy, &device,
             0, 0, SRCCOPY
            );
}

int aKDSceneObject::InteractWithGrid(aHexObjectList& /*objList*/, aObjectList& /*list*/)
{
   return -1;
}

/*********************\
* class aKDBackGround *
\*********************/

aKDBackGround::aKDBackGround(CDC& dc)
   : aKDSceneObject(dc, CPoint(0, 0), 
                    CSize(KDFormats::sceneWidth, KDFormats::sceneHeight)
                   )
{
   hexPointVector.clear();
   addPointVector.clear();
   KDFormats::aKDDraw::DrawBack( device );
   KDFormats::aKDDraw::DrawGrid( device, hexPointVector );
   KDFormats::aKDDraw::DrawAdditional( device, addPointVector);
}

CPoint aKDBackGround::GetPlace(const CPoint& point)
{ // HTBC: Make CPoint(-1, -1) constant
   CPoint place;
   place = FindPlaceInVector(point, hexPointVector);
   if (place != CPoint(-1, -1))
      return place;
   place = FindPlaceInVector(point, addPointVector);
   if (place != CPoint(-1, -1))
      return place;
   return CPoint(-1, -1);
}

bool aKDBackGround::IsMovable(const CPoint& point)
{
   if (std::find(addPointVector.begin(), addPointVector.end(), point)
       != addPointVector.end()
      )
      return true;
   return false;
}

CPoint aKDBackGround::FindPlaceInVector(const CPoint& point,
                                        KDFormats::aHexPointVector& vect
                                       )
{
   using namespace KDFormats;
   const int size = gridHexSize;
   CPoint realPlace;
   for (KDFormats::aHexPointVector::iterator i = vect.begin();
        i != vect.end(); i++
       )
   {
      CPoint points[KDHexFormat::triAmount];
      points[0] = aHexParam::getHexLeftUpper((*i), size);
      points[1] = aHexParam::getHexUpper((*i), size);
      points[2] = aHexParam::getHexRightUpper((*i), size);
      points[3] = aHexParam::getHexRightLower((*i), size);
      points[4] = aHexParam::getHexLower((*i), size);
      points[5] = aHexParam::getHexLeftLower((*i), size);
      if ( (point.x >= i->x) &&
           (point.x <= points[3].x) &&
           (point.y >= aHexParam::lineApprox(points[0], points[1], point.x)) &&
           (point.y >= aHexParam::lineApprox(points[1], points[2], point.x)) &&
           (point.y <= aHexParam::lineApprox(points[3], points[4], point.x)) &&
           (point.y <= aHexParam::lineApprox(points[4], points[5], point.x))
         )
         return (*i);
   }
   return CPoint(-1, -1); // HTBC: Make CPoint(-1, -1) constant
}

bool aKDBackGround::IsTrash(const CPoint& point)
{
   using namespace KDFormats;
   if (point == addPointVector[hexGet + hexPut])
      return true;
   return false;
}

bool aKDBackGround::IsGet(const CPoint& point)
{
   using namespace KDFormats;
   for (int i = 0; i < hexGet; i++)
   {
      if (point == addPointVector[i])
         return true;
   }
   return false;
}

bool aKDBackGround::IsGrid(const CPoint& point)
{
   using namespace KDFormats;
   aHexPointVector::iterator i = std::find(hexPointVector.begin(),
                                           hexPointVector.end(),
                                           point
                                          );
   if (i == hexPointVector.end())
      return false;
   return true;
}

int aKDBackGround::GridSize() const
{
   return hexPointVector.size();
}

/**************************\
* class aKDMaskSceneObject *
\**************************/

aKDMaskSceneObject::aKDMaskSceneObject(CDC& dc, const CPoint& pos, 
                                       const CSize& viewSize)
   : aKDSceneObject(dc, pos, viewSize)
{
   maskView.CreateCompatibleBitmap( &dc, viewSize.cx, viewSize.cy);
   maskDevice.CreateCompatibleDC( &dc );
   oldBitmap = maskDevice.SelectObject( &maskView );
}

void aKDMaskSceneObject::Draw(CDC& dc)
{
   dc.BitBlt(position.x, position.y, size.cx, size.cy, &maskDevice,
                     0, 0, SRCAND
            );
   dc.BitBlt(position.x, position.y, size.cx, size.cy, &device,
                     0, 0, SRCPAINT
            );
}

aKDMaskSceneObject::~aKDMaskSceneObject()
{
   maskDevice.SelectObject(oldBitmap);
}

/****************\
* class aHexagon *
\****************/

aHexagon::aHexagon(CDC& dc, const CPoint& pos, 
                   const KDHexFormat::aHexColorParam& param
                  )
   : aKDMaskSceneObject(dc, pos, 
      KDFormats::aHexParam::getHexSize(KDFormats::gridHexSize))
   , colorsDefined(false)
{
   aHexagon::CreateAppearance(param);
}

aHexagon::aHexagon(CDC& dc, const CPoint& pos, 
                   const KDHexFormat::aHexColorParam& param,
                   KDHexFormat::hexArray& newColors
                  )
   : aKDMaskSceneObject(dc, pos, 
      KDFormats::aHexParam::getHexSize(KDFormats::gridHexSize))
   , colorsDefined(true)
{
   memcpy(colors, newColors, sizeof(colors));
   aHexagon::CreateAppearance(param);
}

void aHexagon::CreateAppearance(const KDHexFormat::aHexColorParam& param)
{
   int i;
   DrawMask();
   COLORREF clBlack = RGB(0, 0, 0);
   CBrush brBlack(clBlack);
   device.FillRect(CRect(CPoint(0, 0), size),
                    &brBlack
                   );
   for (i = 0; i < KDHexFormat::triAmount; i++)
   {
      if (!colorsDefined)
      {
         if (rand() % 100 < KDFormats::grayTriangle)
            colors[i] = 0;
            else 
            colors[i] = 1 + rand() % (param.GetColorsAmount() - 1);
      }
      param.DrawTriangle(device, i, CPoint(0, 0),
                         KDFormats::gridHexSize,
                         colors[i]
                        );
   }
   KDFormats::aKDDraw::DrawHexagon(device, CPoint(0,0),
                                   KDFormats::gridHexSize,
                                   clBlack
                                  );
   colorsDefined = true;
}

void aHexagon::DrawMask()
{
   COLORREF clWhite = RGB(255, 255, 255);
   CBrush brWhite(clWhite);
   COLORREF clBlack = RGB(0, 0, 0);
   maskDevice.FillRect(CRect(CPoint(0, 0), size),
                       &brWhite
                      );
   KDFormats::aKDDraw::DrawSceneHexagon(maskDevice, CPoint(0,0),
                                        KDFormats::gridHexSize,
                                        clBlack, clBlack
                                       );
   KDFormats::aKDDraw::DrawHexagon(maskDevice, CPoint(0,0),
                                   KDFormats::gridHexSize,
                                   clWhite
                                  );
}

void aHexagon::Rotate(bool direction, 
                      const KDHexFormat::aHexColorParam& param
                     )
{
   COLORREF clBlack = RGB(0, 0, 0);
   COLORREF tmp = colors[0];
   int i;
   if (direction)
   {
      for (i = 0; i < KDHexFormat::triAmount; i++)
         colors[i] = colors[i + 1];
      colors[KDHexFormat::triAmount - 1] = tmp;
   }
   for (i = 0; i < KDHexFormat::triAmount; i++)
   {
      param.DrawTriangle(device, i, CPoint(0, 0),
                         KDFormats::gridHexSize,
                         colors[i]
                        );
   }
   KDFormats::aKDDraw::DrawHexagon(device, CPoint(0,0),
                                   KDFormats::gridHexSize,
                                   clBlack
                                  );
}

int aHexagon::GetColor(int hexTri)
{
   return colors[hexTri];
}

int aHexagon::InteractWithGrid(aHexObjectList& objList, aObjectList& /*list*/)
{
   using namespace KDFormats;
   aHexObjectList::iterator i;
   int colorConcur = 0;
   CPoint tmpPoint;
   CPoint point = GetPosition();
   for (i = objList.begin(); i != objList.end(); i++)
   {
      if (point == (*i)->GetPosition())
         return -1;

      tmpPoint = CPoint(point.x - aHexParam::getsqr3div2(2 * gridHexSize),
                        point.y
                       );
       if ((*i)->GetPosition() == tmpPoint)
       {
          if (((*i)->GetColor(4) != (*this).GetColor(1)) &&
              ((*i)->GetColor(4) != 0) && ((*this).GetColor(1) != 0))
             return -1;
          colorConcur++;
          continue;
       }

       tmpPoint = CPoint(point.x - aHexParam::getsqr3div2(gridHexSize),
                         point.y - 3 * gridHexSize /2
                        );
       if ((*i)->GetPosition() == tmpPoint)
       {
          if (((*i)->GetColor(3) != (*this).GetColor(0)) &&
              ((*i)->GetColor(3) != 0) && ((*this).GetColor(0) != 0))
             return -1;
          colorConcur++;
          continue;
       }

       tmpPoint = CPoint(point.x + aHexParam::getsqr3div2(gridHexSize),
                         point.y - 3 * gridHexSize /2
                        );
       if ((*i)->GetPosition() == tmpPoint)
       {
          if (((*i)->GetColor(2) != (*this).GetColor(5)) &&
              ((*i)->GetColor(2) != 0) && ((*this).GetColor(5) != 0))
             return -1;
          colorConcur++;
          continue;
       }

       tmpPoint = CPoint(point.x + aHexParam::getsqr3div2(2 * gridHexSize),
                         point.y
                        );
       if ((*i)->GetPosition() == tmpPoint)
       {
          if (((*i)->GetColor(1) != (*this).GetColor(4)) &&
              ((*i)->GetColor(1) != 0) && ((*this).GetColor(4) != 0))
             return -1;
          colorConcur++;
          continue;
       }

       tmpPoint = CPoint(point.x + aHexParam::getsqr3div2(gridHexSize),
                         point.y + 3 * gridHexSize /2
                        );
       if ((*i)->GetPosition() == tmpPoint)
       {
          if (((*i)->GetColor(0) != (*this).GetColor(3)) &&
              ((*i)->GetColor(0) != 0) && ((*this).GetColor(3) != 0))
             return -1;
          colorConcur++;
          continue;
       }

       tmpPoint = CPoint(point.x - aHexParam::getsqr3div2(gridHexSize),
                         point.y + 3 * gridHexSize /2
                        );
       if ((*i)->GetPosition() == tmpPoint)
       {
          if (((*i)->GetColor(5) != (*this).GetColor(2)) &&
              ((*i)->GetColor(5) != 0) && ((*this).GetColor(2) != 0))
             return -1;
          colorConcur++;
          continue;
       }

   }

   objList.push_back(this);
   return colorConcur;
}

/********************\
* class aGrayHexagon *
\********************/

aGrayHexagon::aGrayHexagon(CDC& dc, const CPoint& pos,
         const KDHexFormat::aHexColorParam& param
        )
   : aHexagon(dc, pos, param)
{
   aGrayHexagon::CreateAppearance(param);
}

void aGrayHexagon::Rotate(bool /*direction*/,
               const KDHexFormat::aHexColorParam& /*param*/
              )
{
}

void aGrayHexagon::CreateAppearance(const KDHexFormat::aHexColorParam& param)
{
   int i;
   DrawMask();
   COLORREF clBlack = RGB(0, 0, 0);
   COLORREF clIn = RGB(200, 200, 200);
   for (i = 0; i < KDHexFormat::triAmount; i++)
      colors[i] = 0;
   KDFormats::aKDDraw::DrawSceneHexagon(device, CPoint(0,0),
                                        KDFormats::gridHexSize,
                                        param.GetColor(0), clIn
                                       );
   KDFormats::aKDDraw::DrawHexagon(device, CPoint(0,0),
                                   KDFormats::gridHexSize,
                                   clBlack
                                  );
}