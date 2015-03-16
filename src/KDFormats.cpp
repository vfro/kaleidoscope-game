#include "StdAfx.h"

#include <math.h>
#include "KDFormats.h"

namespace KDFormats
{
   /* External parameters */
   const int sceneWidth = 500;
   const int sceneHeight = 400;
   const int sceneTop = 40;
   const int sceneLeft = 20;
   const int sceneXBrink = 25;
   const int sceneYBrink = 80;
   const int sceneBegin = 14;
   const int sceneAct = 10;
   const int sceneActWidth = 70;
   const int sceneActScore = 100;
   const int sceneActHeight = 30;
   const int sceneShad = 5;

   const int aboutLeft = 50;
   const int aboutTop = 50;
   const int aboutWidth = 400;
   const int aboutHeight = 300;
   const int aboutHexLeft = 320;
   const int aboutHexTop = 20;
   const int aboutSize = 35;
   const int aboutOkLeft = 160;
   const int aboutOkTop = 250;

   const int hiButtonLeft = 20;
   const int hiButtonTop = 37;
   const int hiLeft = 130;
   const int hiTop = 50;
   const int hiWidth = 270;
   const int hiHeight = 320;
   const int hiOkLeft = 90;
   const int hiOkTop = 270;


   const int sceneSelColor = 90;
   const int sceneSelColorWidth = 150;
   const int sceneSelColorHeight = 30;

   const int nameTop = 5;
   const int nameLeft = 19;
   const int nameWidth = 350;
   const int nameHeight = 20;

   const int closeTop = 5;
   const int closeLeft = 500;
   const int closeWidth = 20;
   const int closeHeight = 20;

   const int minTop = 5;
   const int minLeft = 460;
   const int minWidth = 20;
   const int minHeight = 20;

   const int gridTop = 20;
   const int gridLeft = 20;
   const int gridWidth = 440;
   const int gridHeight = 280;
   const int gridHexSize = 20;
   const int hexGet = 3;
   const int hexPut = 3;
   const int hexTrash = 1;
   const int minColors = 4;
   const int procentGray = 3;
   const int grayTriangle = 20;


   /* Internal parameters */
   const int penWidth = 2;

   const int addHexLeft = 20;
   const int addHexTop = 340;
   const int hexDelta = 2;
   const int hexPixelDelta = 14;

   /* Local Color Defines : */
   #define clBlack RGB(0, 0, 0)
   #define clBack RGB(10, 30, 20)

   #define clGrid RGB(0, 150, 0)
   #define clGridFrom RGB(0, 0, 0)
   #define clGridTo RGB(0, 50, 0)

   #define clGet RGB(0, 200, 200)
   #define clGetFrom RGB(0, 0, 0)
   #define clGetTo RGB(0, 0, 100)

   #define clPut RGB(200, 200, 0)
   #define clPutFrom RGB(30, 30, 0)
   #define clPutTo RGB(100, 100, 0)

   #define clTrash RGB(200, 0, 0)
   #define clTrashFrom RGB(100, 0, 0)
   #define clTrashTo RGB(0, 0, 0)

   int aHexParam::getsqr3div2(int param)
   {
      return static_cast<int>( param * sqrt(3.0) / 2.0 );
   }

   // y = (y2-y1)*(x-x1)/(x2-x1) + y1; (y2-y1)/(y-y1) = (x2-x1)/(x-x1)
   int aHexParam::lineApprox(CPoint point1, CPoint point2, int x)
   {
      if (point2.x == point1.x)
      {
         ASSERT(false);
         return 0;
      }
      if (point2.y == point1.y)
      {
         ASSERT(false);
         return 0;
      }
      if (x != point1.x)
         return (point2.y - point1.y) * (x - point1.x) 
                / (point2.x - point1.x) + point1.y;
         else
         return (point1.y - point2.y) * (x - point2.x) 
                / (point1.x - point2.x) + point2.y;
   }

   CPoint aHexParam::getHexCenter(const CPoint& pos, int size)
   {
      return CPoint(pos.x + getsqr3div2( size ), pos.y + size);
   }

   CPoint aHexParam::getHexUpper(const CPoint& pos, int size)
   {
      return CPoint(pos.x + getsqr3div2( size ), pos.y);
   }

   CPoint aHexParam::getHexLeftUpper(const CPoint& pos, int size)
   {
      return CPoint(pos.x, pos.y + size / 2);
   }

   CPoint aHexParam::getHexLeftLower(const CPoint& pos, int size)
   {
      return CPoint(pos.x, pos.y + 3 * size / 2);
   }

   CPoint aHexParam::getHexLower(const CPoint& pos, int size)
   {
      return CPoint(pos.x + getsqr3div2( size ), pos.y + 2 * size);
   }

   CPoint aHexParam::getHexRightLower(const CPoint& pos, int size)
   {
      return CPoint(pos.x + 2 * getsqr3div2(size), pos.y + 3 * size / 2 );
   }

   CPoint aHexParam::getHexRightUpper(const CPoint& pos, int size)
   {
      return CPoint(pos.x + 2 * getsqr3div2(size), pos.y + size / 2);
   }

   CSize aHexParam::getHexSize(int size)
   {
      return CSize(2 * getsqr3div2( size ), 2 * size);
   }

   void aKDDraw::DrawHexagon(CDC& dc,const CPoint& pos, int size, COLORREF color)
   {
      CPen pen(PS_SOLID, penWidth, color);
      CPen *oldPen = dc.SelectObject(&pen);
      dc.MoveTo(aHexParam::getHexLeftUpper(pos, size));
      dc.LineTo(aHexParam::getHexLeftLower(pos, size));
      dc.LineTo(aHexParam::getHexLower(pos, size));
      dc.LineTo(aHexParam::getHexRightLower(pos, size));
      dc.LineTo(aHexParam::getHexRightUpper(pos, size));
      dc.LineTo(aHexParam::getHexUpper(pos, size));
      dc.LineTo(aHexParam::getHexLeftUpper(pos, size));
      dc.SelectObject(oldPen);
   }

   void aKDDraw::DrawSceneHexagon(CDC& dc,const CPoint& pos, 
                                  int size, COLORREF fromColor, COLORREF toColor
                                  )
   {
      int i;
      COLORREF color = fromColor;
      int r = GetRValue(fromColor);
      int g = GetGValue(fromColor);
      int b = GetBValue(fromColor);
      int dr = ((GetRValue(toColor) - r) / size);
      int dg = ((GetGValue(toColor) - g) / size);
      int db = ((GetBValue(toColor) - b) / size);
      for (i = size; i > 0; i--)
      {
         aKDDraw::DrawHexagon(dc, CPoint(pos.x + static_cast<int>
                                                 ((sqrt(3.0) / 2.0)*(size - i)),
                                  pos.y + (size - i)
                                        ),
                              i, color);
         r += dr;
         g += dg;
         b += db;
         color = RGB(r, g, b);
      }
   }

   void aKDDraw::DrawGrid(CDC& dc, aHexPointVector& hexPointVector)
   {
      int i;
      int j;
      int xDelta = aHexParam::getHexSize(gridHexSize).cx;
      int yDelta = aHexParam::getHexSize(gridHexSize).cy + gridHexSize;
      int xFrom = gridLeft + aHexParam::getHexSize(gridHexSize).cx / 2;
      int yFrom = gridTop + ( 3 * gridHexSize ) / 2;

      /* Draw grid : */ // HTBC: Optimize grid generation
      for (i = gridLeft; i < (gridLeft + gridWidth); i += xDelta)
         for(j = gridTop; j < (gridTop + gridHeight); j += yDelta)
         {
            DrawSceneHexagon(dc, CPoint(i , j), gridHexSize,
                             clGridFrom, clGridTo);
            DrawHexagon(dc, CPoint(i , j), gridHexSize,
                        clGrid);
            hexPointVector.push_back(CPoint(i, j));
         }
      for (i = gridLeft + aHexParam::getsqr3div2(gridHexSize); i < (gridLeft + gridWidth); i += xDelta)
         for(j = gridTop + 3 * gridHexSize / 2; j < (gridTop + gridHeight); j += yDelta)
         {
            DrawSceneHexagon(dc, CPoint(i , j), gridHexSize,
                             clGridFrom, clGridTo);
            DrawHexagon(dc, CPoint(i , j), gridHexSize,
                        clGrid);
            hexPointVector.push_back(CPoint(i, j));
         }
   }

   void aKDDraw::DrawAdditional(CDC& dc, aHexPointVector& hexPointVector)
   {
      /*Draw additional hexagons : */
      int i = 0;
      while (i < hexGet + hexPut + hexTrash)
      {
         int hexX;
         int hexY = addHexTop;
         COLORREF color;
         COLORREF clFrom;
         COLORREF clTo;
         if (i < hexGet)
         {
            hexX = i * aHexParam::getHexSize(gridHexSize).cx;
            color = clGet;
            clFrom = clGetFrom;
            clTo = clGetTo;
         } else
         if ((i >= hexGet) && (i < hexGet + hexPut))
         {
            hexX = (i + hexDelta) * aHexParam::getHexSize(gridHexSize).cx;
            color = clPut;
            clFrom = clPutFrom;
            clTo = clPutTo;
         } else
           {
               hexX = (i + 2 * hexDelta) * aHexParam::getHexSize(gridHexSize).cx;
               color = clTrash;
               clFrom = clTrashFrom;
               clTo = clTrashTo;
           }
         hexX += addHexLeft + i * hexPixelDelta;
         DrawSceneHexagon(dc, CPoint(hexX , hexY), gridHexSize,
                          clFrom, clTo);
         DrawHexagon(dc, CPoint(hexX , hexY), gridHexSize,
                     color);
         hexPointVector.push_back(CPoint(hexX, hexY));
         i++;
      }
   }

   void aKDDraw::DrawBack(CDC& dc)
   {
      /*
      int size = sceneWidth * sceneHeight << 2;
      int *buff = new int[size];
      for ( int i = 0; i < size; i++ )
         buff[i] = RGB( 0, rand() % 50, 0 );
      dc.GetCurrentBitmap()->SetBitmapBits( size, buff );
      delete buff;
      */
      COLORREF color = RGB(0, 0, 0);
      CPen pen(PS_SOLID, 1, color);
      CPen* oldPen = dc.SelectObject(&pen);

      for (int i = 0; i < sceneHeight; i++)
      {
         int koef = i * 20 / sceneHeight;
         color = RGB(0, 40 - koef, 0);
         CPen newPen(PS_SOLID, 1, color);
         dc.SelectObject(&newPen);
         dc.MoveTo(0, i);
         dc.LineTo(sceneWidth, i);
      }

      dc.SelectObject(oldPen);
   }
};