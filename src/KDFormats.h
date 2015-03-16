#ifndef KDFORMATS
#define KDFORMATS

namespace KDFormats
{
   extern const int sceneWidth;
   extern const int sceneHeight;
   extern const int sceneTop;
   extern const int sceneLeft;
   extern const int sceneXBrink;
   extern const int sceneYBrink;
   extern const int sceneBegin
      ;
   extern const int sceneAct;
   extern const int sceneActWidth;
   extern const int sceneActScore;
   extern const int sceneActHeight;

   extern const int sceneSelColor;
   extern const int sceneSelColorWidth;
   extern const int sceneSelColorHeight;

   extern const int aboutLeft;
   extern const int aboutTop;
   extern const int aboutWidth;
   extern const int aboutHeight;
   extern const int aboutHexLeft;
   extern const int aboutHexTop;
   extern const int aboutSize;
   extern const int aboutOkLeft;
   extern const int aboutOkTop;

   extern const int hiButtonLeft;
   extern const int hiButtonTop;
   extern const int hiWidth;
   extern const int hiHeight;
   extern const int hiOkLeft;
   extern const int hiOkTop;
   extern const int hiLeft;
   extern const int hiTop;

   extern const int sceneShad;

   extern const int nameTop;
   extern const int nameLeft;
   extern const int nameWidth;
   extern const int nameHeight;

   extern const int closeTop;
   extern const int closeLeft;
   extern const int closeWidth;
   extern const int closeHeight;

   extern const int minTop;
   extern const int minLeft;
   extern const int minWidth;
   extern const int minHeight;

   extern const int gridTop;
   extern const int gridLeft;
   extern const int gridWidth;
   extern const int gridHeight;
   extern const int gridHexSize;
   extern const int hexGet;
   extern const int hexPut;
   extern const int hexTrash;
   extern const int minColors;
   extern const int procentGray;
   extern const int grayTriangle;
//   extern const int procentBomb;

   typedef std::vector<CPoint> aHexPointVector;

   class aKDDraw
   {
   public:
      static void DrawHexagon(CDC& dc,const CPoint& pos, int size, COLORREF color);
      static void DrawSceneHexagon(CDC& dc,const CPoint& pos, 
                                   int size, COLORREF fromColor, COLORREF toColor
                                  );
      static void DrawGrid(CDC& dc, aHexPointVector& hexPointVector);
      static void DrawAdditional(CDC& dc, aHexPointVector& hexPointVector);
      static void DrawBack(CDC& dc);
   private:
      aKDDraw(){};
   };

   class aHexParam
   {
   public:
      static CPoint getHexCenter(const CPoint& pos, int size);
      static CPoint getHexUpper(const CPoint& pos, int size);
      static CPoint getHexLeftUpper(const CPoint& pos, int size);
      static CPoint getHexLeftLower(const CPoint& pos, int size);
      static CPoint getHexLower(const CPoint& pos, int size);
      static CPoint getHexRightLower(const CPoint& pos, int size);
      static CPoint getHexRightUpper(const CPoint& pos, int size);
      static CSize getHexSize(int size);
      static int getsqr3div2(int param);
      // y = (y2-y1)*(x-x1)/(x2-x1) + y1
      static int lineApprox(CPoint point1, CPoint point2, int x);
   private:
      aHexParam(){};
   };
};

#endif