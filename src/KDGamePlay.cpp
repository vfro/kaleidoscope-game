#include "StdAfx.h"

#include "KDGamePlay.h"
#include "KDFormats.h"

aGamePlay::aGamePlay()
   : state(false),
     trashed(0)
{
}

void aGamePlay::StartGame()
{
   state = true;
   score = 0;
   trashed = 0;
}

void aGamePlay::StopGame()
{
   state = false;
}

void aGamePlay::Draw(CDC& dc, const KDHexFormat::aHexColorParam& /*param*/)
{
   using namespace KDFormats;

   COLORREF textColor = RGB(0, 150, 150);
   COLORREF textFailColor = RGB(150, 0, 0);
   COLORREF clShad = RGB(150, 0, 0);
   COLORREF clFailShad = RGB(0, 0, 0);

   UINT oldAlign = dc.SetTextAlign(TA_LEFT | TA_TOP);
   UINT oldMode = dc.SetBkMode(TRANSPARENT);

   CString message;

   CFont font;
   CFont* oldFont;

   font.CreateFont(16, 8, 0, 0, FW_BOLD, false, false,
                   0,ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
                   CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
                   DEFAULT_PITCH | FF_DECORATIVE,"Times");
   oldFont = dc.SelectObject(&font);
   if (state)
   {
      dc.SetTextColor(textColor);
      message.Format("Score : %i", score);
      dc.SetTextColor(clShad);
      dc.TextOut(gridLeft + 2, 4, message);
      dc.SetTextColor(textColor);
      dc.TextOut(gridLeft, 2, message);
   } else
   {
      message = "Game Failed.";
      dc.SetTextColor(clFailShad);
      dc.TextOut(gridLeft + 2, 4, message);
      dc.SetTextColor(textFailColor);
      dc.TextOut(gridLeft, 2, message);
   }
   dc.SelectObject(oldFont);
   dc.SetBkMode(oldMode);
   dc.SetTextAlign(oldAlign);
}

void aGamePlay::AddHexagon(int coincide, const KDHexFormat::aHexColorParam& param)
{
   int addScore = 1;
   int i;
   if (coincide == -1)
      return;
   for (i = 0; i < coincide; i++)
      addScore *= 2;
   score += (param.GetColorsAmount() - KDFormats::minColors) * addScore;
}

void aGamePlay::InTrash(const KDHexFormat::aHexColorParam& /*param*/)
{
   int addScore = 0;
   trashed++;
   /*
   for (i = 0; i < trashed; i++)
      addScore -= trashed;
   */
   addScore -= trashed*trashed;
   score += addScore;
   if (score < 0)
      state = false;
}

int aGamePlay::GetScore() const
{
   return score;
}

void aGamePlay::SetScore(int newScore)
{
   score = newScore;
}

int aGamePlay::GetTrashed() const
{
   return trashed;
}

void aGamePlay::SetTrashed(int newTrashed)
{
   trashed = newTrashed;
}
