#include "StdAfx.h"

#include "KDHexFormat.h"
#include "KDFormats.h"

namespace KDHexFormat
{
   COLORREF hexColors[3][7] = 
         { { RGB(100, 100, 100), RGB(130, 60, 0),   RGB(100, 175, 250),  RGB(125, 250, 125),
             RGB(250, 125, 250), RGB(10,  50, 150), RGB(100, 0, 100)
           },
           { RGB(100, 100, 100), RGB(50, 200, 255), RGB(250, 130, 250),  RGB(250, 250, 0),
             RGB(100, 0, 0),     RGB(50, 0, 100),   RGB(250, 40, 80)
           },
           { RGB(100, 100, 100), RGB(200, 0, 0),    RGB(0, 0, 200),      RGB(0, 200, 200),
             RGB(200, 200, 0),   RGB(200, 0, 200),  RGB(250, 120, 0)
           }
         };

   aHexColorParam::aHexColorParam(int cAmount)
      : colorsAmount(cAmount)
      , colorsArray(0)
   {
      CustomizeColors(cAmount);
      srand( (unsigned)time( 0 ) );
   }

   aHexColorParam::~aHexColorParam()
   {
   }

   int aHexColorParam::GetColorsArray() const
   {
      return colorsArray;
   }

   void aHexColorParam::CustomizeColors(int cAmount, int colorsArraySet)
   {
      colorsAmount = cAmount;

      if (colorsArraySet == -1)
         colorsArray = rand() % 3;
      else
         colorsArray = colorsArraySet;

      int i;
      for (i = 0; i < colorsAviable; i++)
         colors[i] = hexColors[colorsArray][i];
   }

   const COLORREF& aHexColorParam::GetColor(int color) const
   {
      return colors[color];
   }

   int aHexColorParam::GetColorsAmount() const
   {
      return colorsAmount;
   }

   void aHexColorParam::DrawTriangle(CDC& dc, int tri,
                                     const CPoint& pos, int size,
                                     int color) const
   {
      using namespace KDFormats;
      CPoint point1 = CPoint(0, 0);
      CPoint point2 = CPoint(0, 0);
      CPoint point3 = aHexParam::getHexCenter(pos, size);;
      switch(tri)
      {
      case leftUpper :
         point1 = aHexParam::getHexUpper(pos, size);
         point2 = aHexParam::getHexLeftUpper(pos, size);
         break;
      case leftMidle :
         point1 = aHexParam::getHexLeftUpper(pos, size);
         point2 = aHexParam::getHexLeftLower(pos, size);
         break;
      case leftLower :
         point1 = aHexParam::getHexLeftLower(pos, size);
         point2 = aHexParam::getHexLower(pos, size);
         break;
      case rightUpper :
         point1 = aHexParam::getHexLower(pos, size);
         point2 = aHexParam::getHexRightLower(pos, size);
         break;
      case rightMidle :
         point1 = aHexParam::getHexRightLower(pos, size);
         point2 = aHexParam::getHexRightUpper(pos, size);
         break;
      case rightLower :
         point1 = aHexParam::getHexRightUpper(pos, size);
         point2 = aHexParam::getHexUpper(pos, size);
         break;
      default :
         ASSERT(false);
         break;
      }
      DrawPointsTriangle(dc, point1, point2, point3, color);
   }

   void aHexColorParam::DrawPointsTriangle(CDC& dc,
                                     const CPoint& point1,
                                     const CPoint& point2,
                                     const CPoint& point3,
                                     int color
                                     ) const
   {
      COLORREF clBlack = RGB(0, 0, 0);
      COLORREF clEdge = RGB(0, 100, 0);
      CPen* oldPen;
      CPen pen(PS_SOLID, 2, clEdge);
      const int pointsCount = 3;
      CPoint points[pointsCount];
      points[0] = point1;
      points[1] = point2;
      points[2] = point3;
      CRgn rgn;
      rgn.CreatePolygonRgn(points, pointsCount, ALTERNATE /*WINDING*/);
      CBrush brush(GetColor(color));
      dc.FillRgn( &rgn, &brush);
      oldPen = dc.SelectObject(&pen);
      dc.MoveTo(point1);
      dc.LineTo(point3);
      dc.LineTo(point2);
      dc.SelectObject( oldPen );
   }
}