#ifndef KDGAMEPLAY
#define KDGAMEPLAY

#include "KDHexFormat.h"

class aGamePlay
{
public:
   aGamePlay();
   void StartGame();
   void StopGame();

   void AddHexagon(int coincide, const KDHexFormat::aHexColorParam& param);
   void Draw(CDC& dc, const KDHexFormat::aHexColorParam& param);
   void InTrash(const KDHexFormat::aHexColorParam& param);

   int GetScore() const;
   void SetScore(int newScore);

   int GetTrashed() const;
   void SetTrashed(int newTrashed);
private:
   bool state;
   int score;
   int trashed;
};

#endif