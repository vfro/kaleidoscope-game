#ifndef KDHEXFORMAT
#define KDHEXFORMAT

namespace KDHexFormat
{
   enum aHexTriangle
   {
      leftUpper = 0,
      leftMidle = 1,
      leftLower = 2,
      rightUpper = 3,
      rightMidle = 4,
      rightLower = 5
   };

   enum aTriParam
   {
      triAmount = 6,
      colorsAviable = 7
   };

   typedef int hexArray[triAmount];

   class aHexColorParam
   {
   public:
      aHexColorParam(int cAmount);
      virtual ~aHexColorParam();

      void CustomizeColors(int cAmount, int colorsArraySet = -1);
      int GetColorsArray() const;

      const COLORREF& GetColor(int color) const;
      int GetColorsAmount() const;
      void DrawTriangle(CDC& dc, int tri,
                        const CPoint& pos, int size,
                        int color) const;
   private:
      void DrawPointsTriangle(CDC& dc,
                              const CPoint& point1,
                              const CPoint& point2,
                              const CPoint& point3,
                              int color
                             ) const;
      COLORREF colors[colorsAviable];
      int colorsAmount;
      int colorsArray;
   };
}

#endif