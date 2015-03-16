#ifndef KDSCENEOBJECT
#define KDSCENEOBJECT

#include "KDHexFormat.h"
#include "KDFormats.h"

class aKDSceneObject;
class aHexagon;

typedef std::list<aKDSceneObject*> aObjectList;
typedef std::list<aHexagon*> aHexObjectList;
typedef aHexObjectList aRotateObjectList;

class aKDSceneObject
{
public:
   aKDSceneObject(CDC& dc, const CPoint& pos, const CSize& viewSize);
   virtual ~aKDSceneObject();
   virtual void Draw(CDC& dc);
   virtual const CPoint& GetPosition() const;
   virtual const CSize& GetSize() const;
   virtual void ReMove(const CPoint& pos);
   virtual int InteractWithGrid(aHexObjectList& objList, aObjectList& list);
protected:
   CBitmap view;
   CBitmap* oldBitmap;
   CDC device;
   CPoint position;
   CSize size;
};

class aKDBackGround : public aKDSceneObject
{
public:
   aKDBackGround(CDC& dc);
   CPoint GetPlace(const CPoint& point);
   bool IsMovable(const CPoint& point);
   bool IsTrash(const CPoint& point);
   bool IsGet(const CPoint& point);
   bool IsGrid(const CPoint& point);
   int GridSize() const;
private:
   CPoint FindPlaceInVector(const CPoint& point, KDFormats::aHexPointVector& vect);
   KDFormats::aHexPointVector hexPointVector;
   KDFormats::aHexPointVector addPointVector;
};

class aKDMaskSceneObject : public aKDSceneObject
{
public:
   aKDMaskSceneObject(CDC& dc, const CPoint& pos, const CSize& viewSize);
   virtual ~aKDMaskSceneObject();
   virtual void Draw(CDC& dc);
protected:
   CBitmap maskView;
   CDC maskDevice;
};

class aHexagon : public aKDMaskSceneObject
{
public:
   aHexagon(CDC& dc, const CPoint& pos,
            const KDHexFormat::aHexColorParam& param
           );
   aHexagon(CDC& dc, const CPoint& pos,
            const KDHexFormat::aHexColorParam& param,
            KDHexFormat::hexArray& newColors
           );
   virtual void Rotate(bool direction,
               const KDHexFormat::aHexColorParam& param
              );
   int GetColor(int hexTri);
   virtual int InteractWithGrid(aHexObjectList& objList, aObjectList& list);
protected:
   void DrawMask();
   virtual void CreateAppearance(const KDHexFormat::aHexColorParam& param);
   KDHexFormat::hexArray colors;
   bool colorsDefined;
};

class aGrayHexagon : public aHexagon
{
public:
   aGrayHexagon(CDC& dc, const CPoint& pos,
            const KDHexFormat::aHexColorParam& param
           );
   virtual void Rotate(bool direction,
               const KDHexFormat::aHexColorParam& param
              );
protected:
   void CreateAppearance(const KDHexFormat::aHexColorParam& param);
};

#endif