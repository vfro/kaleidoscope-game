#include "StdAfx.h"
#include "KDFormats.h"
#include "Controls.h"

aKDControl::aKDControl(const CPoint& pos, const CSize& viewSize)
   : size(viewSize),
     position(pos)
{
}

aKDControl::~aKDControl()
{
   device.SelectObject(oldBitmap);
}

void aKDControl::SelectDC(CDC& dc)
{
   view.CreateCompatibleBitmap( &dc, size.cx, size.cy );
   device.CreateCompatibleDC( &dc );
   oldBitmap = device.SelectObject( &view );
}

const CPoint& aKDControl::GetPosition() const
{
   return position;
}

const CSize& aKDControl::GetSize() const
{
   return size;
}

void aKDControl::ReMove(const CPoint& pos)
{
   position = pos;
}

void aKDControl::Draw(CDC& dc)
{
   dc.BitBlt(position.x, position.y, size.cx * 2, size.cy * 2, &device,
             0, 0, SRCCOPY
            );
}

bool aKDControl::IsIn(const CPoint& point)
{
   if ((point.x > position.x) && (point.x < position.x + size.cx) &&
       (point.y > position.y) && (point.y < position.y + size.cy)
      )
      return true;
   return false;
}

/***********************\
* class aMainBackGround *
\***********************/

aMainBackGround::aMainBackGround(const CPoint& pos, const CSize& viewSize)
   : aKDControl(pos,viewSize)
{
}

void aMainBackGround::SelectDC(CDC& dc)
{
   anAncestor::SelectDC(dc);
   CreateAppearance();
}

void aMainBackGround::CreateAppearance()
{
   using namespace KDFormats;
   int i;
   /*int j;*/
   int oldAlign = device.SetTextAlign(TA_LEFT | TA_TOP);
   int oldMode = device.SetBkMode(TRANSPARENT);

   COLORREF color;
   COLORREF textColor = RGB(170, 170, 0);
   COLORREF closeColor = RGB(100, 0, 0);
   COLORREF closeBColor = RGB(200, 0, 0);
   COLORREF minColor = RGB(0, 50, 100);
   COLORREF minBColor = RGB(0, 100, 200);
   COLORREF mainColor = RGB(0, 0, 200);

   CBrush closeBrush(closeColor);
   CBrush minBrush(minColor);
   CBrush* oldBrush;

   CPen closePen(PS_SOLID, 2, closeBColor);
   CPen minPen(PS_SOLID, 2, minBColor);
   CPen mainPen(PS_SOLID, 2, mainColor);
   CPen pen(PS_SOLID, 2, RGB(0, 100, 0));
   CPen* oldPen;

   CFont font;
   CFont* oldFont;

   oldPen = device.SelectObject(&mainPen);
   device.Rectangle(2, sceneBegin + 2, size.cx - 2, size.cy - 2);

   /*
   CDC tdc; tdc.CreateCompatibleDC( &device );
   CBitmap tbmp; tbmp.CreateCompatibleBitmap( &device, size.cx, size.cy );
   int bsize = size.cx * size.cy << 2;
   int *buff = new int[bsize];
   for ( i = 0; i < bsize; i++ )
      buff[i] = RGB( rand() % 100, 0, 0 );
   tbmp.SetBitmapBits( bsize, buff );
   tdc.SelectObject( &tbmp );
   device.BitBlt( 3, 3, size.cx - 7, size.cy - 7, &tdc, 0, 0, SRCCOPY );
   delete buff;
   */

   /*
   for (i = 3; i < size.cx - 4; i++)
      for (j = 3; j < size.cy - 4; j++)
      {
         color = RGB(0, 0, 60 + (j * 4 / size.cy));
         device.SetPixel(i,j,color);
      }
   */

   for(i = sceneBegin + 3; i < size.cy - 4; ++i)
   {
      CPen linePen(PS_SOLID, 0, RGB(0, 0, 60 + (i * 4 / size.cy)));
      device.SelectObject(linePen);
      device.MoveTo(3, i);
      device.LineTo(size.cx - 4, i);
   }

   device.MoveTo(2, sceneBegin + 2);
   device.LineTo(size.cx - 2, sceneBegin + 2);

   device.SelectObject(&pen);

   device.MoveTo(sceneLeft - 1, sceneTop - 1);
   device.LineTo(sceneLeft - 1, sceneTop + sceneHeight + 1);
   device.LineTo(sceneLeft + sceneWidth + 1, sceneTop + sceneHeight + 1);
   device.LineTo(sceneLeft + sceneWidth + 1, sceneTop - 1);
   device.LineTo(sceneLeft - 1, sceneTop - 1);

   for (i = 0; i < nameHeight; i++)
   {
      if (i < (nameHeight / 2))
         color = RGB(0, i * 10, 0);
         else
         color = RGB(0, (nameHeight - i) * 10, 0);
      CPen namePen(PS_SOLID, 1, color);
      device.SelectObject(&namePen);
      device.MoveTo(nameLeft, nameTop + i);
      device.LineTo(nameLeft + nameWidth, nameTop +  i);
   }

   font.CreateFont(16, 8, 0, 0, FW_BOLD, false, false,
                   0,ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
                   CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
                   DEFAULT_PITCH | FF_DECORATIVE,"Times");
   oldFont = device.SelectObject(&font);
   device.SetTextColor(textColor);
   device.TextOut(nameLeft + 5, nameTop + 2, "Kaleidoscope");

   oldBrush = device.SelectObject(&closeBrush);
   device.SelectObject(&closePen);
   device.Rectangle(CRect(closeLeft, closeTop,
                    closeLeft + closeWidth, closeTop + closeHeight));
   device.MoveTo(closeLeft + 3, closeTop + 3);
   device.LineTo(closeLeft + closeWidth - 5, closeTop + closeHeight - 5);
   device.MoveTo(closeLeft + closeWidth - 5, closeTop + 3);
   device.LineTo(closeLeft + 3, closeTop + closeHeight - 5);

   device.SelectObject(&minBrush);
   device.SelectObject(&minPen);
   device.Rectangle(CRect(minLeft, minTop,
                    minLeft + minWidth, minTop + minHeight));

   device.MoveTo(minLeft + minWidth / 2, minTop + 3);
   device.LineTo(minLeft + minWidth / 2, minTop + minHeight - 5);
   device.MoveTo(minLeft + minWidth / 2 - 4, minTop + minHeight - 9);
   device.LineTo(minLeft + minWidth / 2, minTop + minHeight - 5);
   device.LineTo(minLeft + minWidth / 2 + 3, minTop + minHeight - 9);

   device.SetBkMode(oldMode);
   device.SetTextAlign(oldAlign);
   device.SelectObject(oldFont);
   device.SelectObject(oldPen);
   device.SelectObject(oldBrush);
}

/****************\
* class aButton  *
\****************/

aButton::aButton(const CPoint& pos, const CSize& viewSize,
                 COLORREF clBorder, COLORREF clIn, CString caption,
                 bool shad, bool leftPushed
                )
   : aKDControl(pos, viewSize),
     color(clBorder),
     inColor(clIn),
     text(caption),
     pressed(false),
     pushed(leftPushed),
     shaded(shad),
     selected(false)
{
}

void aButton::SelectDC(CDC& dc)
{
   anAncestor::SelectDC(dc);
   CreateAppearance();
}

void aButton::CreateAppearance()
{
   CPen buttonPen(PS_SOLID, 2, color);
   CPen* oldPen;

   CBrush buttonBrush(inColor);
   CBrush* oldBrush;

   oldPen = device.SelectObject(&buttonPen);
   oldBrush = device.SelectObject(&buttonBrush);
   device.Rectangle(CRect(1, 1, size.cx, size.cy));

   device.SelectObject(oldPen);
   device.SelectObject(oldBrush);
}

void aButton::SetPressedStatus(bool status)
{
   pressed = status;
}

void aButton::Draw(CDC& dc)
{
   using namespace KDFormats;

   CPoint add(0, 0);

   CFont font;
   CFont* oldFont;

   COLORREF shadColor = RGB(10, 10, 80);

   CPen shadPen(PS_SOLID, 1, shadColor);
   CPen* oldPen;

   CBrush shadBrush(shadColor);
   CBrush* oldBrush;

   if (shaded)
   {
      oldBrush = dc.SelectObject(&shadBrush);
      oldPen = dc.SelectObject(&shadPen);

      dc.Rectangle(CRect(position.x+ sceneShad, 
                         position.y + sceneShad,
                         position.x + size.cx + sceneShad,
                         position.y + size.cy + sceneShad
                        )
                  );
      dc.SelectObject(oldPen);
      dc.SelectObject(oldBrush);
   }

   if (pressed)
   {
      if (pushed)
         add += CPoint(sceneShad, sceneShad);
         else
         add += CPoint(- sceneShad, sceneShad);
   }

   position += add;
   anAncestor::Draw(dc);
   position -= add;

   font.CreateFont(16, 8, 0, 0, FW_BOLD, false, false,
                   0,ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
                   CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
                   DEFAULT_PITCH | FF_DECORATIVE,"Times");

   oldFont = dc.SelectObject(&font);
   dc.SetBkMode(TRANSPARENT);
   dc.SetTextColor(RGB(200, 200, 0));

   dc.DrawText(text, 
                   CRect(position.x + add.x, 
                         position.y + size.cy / 2 - 8 + add.y,
                         position.x + size.cx + add.x,
                         position.y + size.cy + add.y),
                         DT_CENTER);

   dc.SelectObject(oldFont);
}

void aButton::SetText(const CString newText)
{
   text = newText;
}

bool aButton::GetPressedStatus() const
{
   return pressed;
}

void aButton::SetSelected(bool status)
{
   selected = status;
}

bool aButton::GetSelected()
{
   return selected;
}

/************************\
* class aSelector        *
\************************/

aSelector::aSelector(const CPoint& pos, const CSize& viewSize, const CString& text)
   : left(pos, CSize(viewSize.cx / 8 - 2, viewSize.cy),
          RGB(0, 150, 0), RGB(0, 50, 0),
          "<", false
         ),
     right(pos + CPoint(viewSize.cx * 7 / 8 + 1, 0),
           CSize(viewSize.cx / 8 - 2, viewSize.cy),
           RGB(0, 150, 0), RGB(0, 50, 0),
           ">", false, false
          ),
     middle(pos + CPoint(viewSize.cx / 8 + 5, viewSize.cy / 2 + 2),
            CSize(viewSize.cx  * 6 / 8 - 10, viewSize.cy / 2 - 2),
            RGB(0, 150, 200), RGB(0, 100, 150),
            "", false
           ),
     caption(pos + CPoint(viewSize.cx / 8 + 5, 0),
            CSize(viewSize.cx  * 6 / 8 - 10, viewSize.cy / 2 - 2),
            RGB(0, 150, 0), RGB(0, 50, 0),
            text, false
           ),
     current(0),
     name(text),
     position(pos),
     size(viewSize),
     enabled(true)
{
}

void aSelector::Draw(CDC& dc)
{
   middle.SetText(selection[current]);
   if (enabled)
   {
      if (current != 0)
         left.Draw(dc);
      if (current < int(selection.size() - 1))
         right.Draw(dc);
   }
   middle.Draw(dc);
   caption.Draw(dc);
}

void aSelector::SelectDC(CDC& dc)
{
   left.SelectDC(dc);
   middle.SelectDC(dc);
   right.SelectDC(dc);
   caption.SelectDC(dc);
}

void aSelector::AddSelection(const CString select)
{
   selection.push_back(select);
}

bool aSelector::IsIn(const CPoint& point)
{
   if ((point.x > position.x) && (point.x < position.x + size.cx) &&
       (point.y > position.y) && (point.y < position.y + size.cy)
      )
      return true;
   return false;
}

void aSelector::Act(const CPoint& point)
{
   if (!enabled)
      return;
   if (right.IsIn(point) && right.GetSelected())
   {
      current++;
      if (current == int(selection.size()))
         current--;
   }
   if (left.IsIn(point) && left.GetSelected())
   {
      current--;
      if (current == -1)
         current++;
   }
   left.SetSelected(false);
   right.SetSelected(false);
}

void aSelector::Push(const CPoint& point)
{
   left.SetSelected(false);
   right.SetSelected(false);
   if (left.IsIn(point))
   {
      left.SetPressedStatus();
      left.SetSelected();
      push = leftButton;
   }
   if (right.IsIn(point))
   {
      push = rightButton;
      right.SetPressedStatus();
      right.SetSelected();
   }
}

void aSelector::UnPush()
{
   left.SetPressedStatus(false);
   right.SetPressedStatus(false);
}

int aSelector::GetSelected()
{
   return current;
}

void aSelector::SetSelected(int selected)
{
   current = selected;
}

void aSelector::Enable(bool status)
{
   enabled = status;
}