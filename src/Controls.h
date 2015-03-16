#ifndef CONTROLS_IDCLUDED
#define CONTROLS_IDCLUDED

class aKDControl
{
public:
   virtual ~aKDControl();
   virtual void SelectDC(CDC& dc);
   virtual void Draw(CDC& dc);
   virtual const CPoint& GetPosition() const;
   virtual const CSize& GetSize() const;
   virtual void ReMove(const CPoint& pos);
   virtual bool IsIn(const CPoint& point);
protected:
   aKDControl(const CPoint& pos, const CSize& viewSize);
   CBitmap view;
   CBitmap* oldBitmap;
   CDC device;
   CPoint position;
   CSize size;
};

class aMainBackGround : public aKDControl
{
public:
   aMainBackGround(const CPoint& pos, const CSize& viewSize);
   virtual void SelectDC(CDC& dc);
protected:
   void CreateAppearance();
private:
   typedef aKDControl anAncestor;
};

class aButton : public aKDControl
{
public:
   aButton(const CPoint& pos, const CSize& viewSize,
           COLORREF clBorder, COLORREF clIn,
           CString caption, bool shad = true, bool leftPushed = true
          );
   virtual void Draw(CDC& dc);
   virtual void SelectDC(CDC& dc);
   void SetPressedStatus(bool status = true);
   bool GetPressedStatus() const;
   void SetText(const CString newText);
   void SetSelected(bool status = true);
   bool GetSelected();
   bool selected;
protected:
   bool pressed;
   void CreateAppearance();
   CString text;
   COLORREF color;
   COLORREF inColor;
   bool shaded;
   bool pushed;   // HTBC: aka direction
private:
   typedef aKDControl anAncestor;
};

class aSelector
{
public:
   aSelector(const CPoint& pos, const CSize& viewSize, const CString& text);
   virtual void Draw(CDC& dc);
   virtual void SelectDC(CDC& dc);
   void AddSelection(const CString select);
   int GetSelected();
   void SetSelected(int selected);
   virtual bool IsIn(const CPoint& point);
   void Push(const CPoint& point);
   void UnPush();
   void Act(const CPoint& point);
   void Enable(bool status = true);
private:
   typedef std::vector<CString> aSelectionList;
   enum Pushed
   {
      nothingPushed,
      leftButton,
      rightButton
   };
   aButton left;
   aButton right;
   aButton middle;
   aButton caption;
   aSelectionList selection;
   int current;
   CString name;
   CPoint position;
   CSize size;
   Pushed push;
   bool enabled;
};

#endif