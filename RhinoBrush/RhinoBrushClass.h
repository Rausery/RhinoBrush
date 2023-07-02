#ifndef CIRCLE_H
#define CIRCLE_H
#pragma once
#include "StdAfx.h"
#include "RhinoBrushPlugIn.h"

//class Operation;
//
//	class Inflate;
//	class ScaleZ;
//	class Flatten;
//
//class Shape;
//
//	class Circle;
//	class PolyGon;
//	class RecTangle;
//	class Customized;
//
//		class RhinoBrush;
//
//			class InflateCircle;
//			class InflatePolygon;
//			class InflateRectangle;
//			class InflateCustomized;
//
//			class ScaleZCircle;
//			class ScaleZPolygon;
//			class ScaleZRectangle;
//			class ScaleZCustomized;
//
//			class FlattenCircle;
//			class FlattenPolygon;
//			class FlattenRectangle;
//			class FlattenCustomized;

/////////////////////////////////////////�������ܺ�������///////////////////////////////////////////////

//���������޸�����Ϊͬ�߶�ƽ�����ߣ�����ø߶�z��
// [in]ON_Curve *cv:ԭʼ���� [out]ON_NurbsCurve* planarcv:�޸ĺ�ĵȸ�ƽ������
HRESULT getZ_Planar(const ON_Curve* cv, ON_NurbsCurve* planarcv);

//����������Ȧ��
// Ӧ�������б�ˢ��ģʽ��������Ȧ�е���inner curve�ľ����outer curve�ľ������z�仯����
	//��out_mode��
	//��ö�٣���Ȧ�༭�������͡�
enum class Out_mode
{
	sin = 0,
	ligth_sin = 1
};
//���������߶���ͬ��������ά��ȡ���롿
inline double distXY(ON_3dPoint& a, ON_3dPoint& b);
// [in&out]surface* , [in] inner curve(0), [in] outer curve(0), [in&out] ���Ƶ�������, 
// [in] ��Ȧ�ڿ��Ƶ�����, [in] ��Ȧ�ڿ��Ƶ�����, [in] Out_mode �㷨
HRESULT OuterEdit(ON_NurbsSurface* srf, const ON_NurbsCurve* inner_cv, const ON_NurbsCurve* outer_cv, ON_3dPoint* ptrCV,
	int* indices_out, int* indices_in, const int& count_out, const int& count_in);

//����������װ��OFFSET��
HRESULT offset_XY(const ON_NurbsCurve* inner_curve, ON_NurbsCurve* offseted, double& distance, ON_3dPoint& center);

//��������getCenter��
HRESULT getCenter(const ON_NurbsCurve* curve, ON_3dPoint& center);

//��������MoveCurve��
// [in] ON_NurbsCurve* curr_curve,[out] ON_NurbsCurve* new_curve [in]ON_3dPoint* Ŀ���
HRESULT moveCurve(const ON_NurbsCurve* curr_curve, ON_NurbsCurve* new_curve, const ON_3dPoint* to_center);

//////////////////////////////////////////////�ඨ��/////////////////////////////////////////////////////////

//Operation�༰����������
class Operation {
public:
	double Strength;

	Operation(double strength) : Strength(strength) {};
	~Operation() = default;
	virtual HRESULT InnerEdit(ON_NurbsSurface* srf, ON_3dPoint* ptrCV, int* indices, const int& count) = 0;
};

class Inflate : public Operation {
public:
	Inflate(double strength) : Operation(strength) {
		if (strength < -99) {
			RhinoApp().Print(L"���Ȳ���С��-99��������Ϊ-99");
			Strength = -99;
		}
		else if (strength > 99) {
			RhinoApp().Print(L"���Ȳ��ô���99��������Ϊ99");
			Strength = 99;
		}
	}
	~Inflate() = default;
	HRESULT InnerEdit(ON_NurbsSurface* srf, ON_3dPoint* ptrCV, int* indices, const int& count) override;
};

// ��ö�٣�ScaleZ���͡�
// Mean:�������е��ƽ���߶ȣ� Mid��������ߵ����͵�߶ȵ�ƽ����
enum class ScaleZ_mode
{
	Mean = 0,
	Mid = 1
};

class ScaleZ : public Operation {
public:
	ScaleZ_mode mode = ScaleZ_mode::Mean;
	ScaleZ(double strength) :Operation(strength) {
		if (strength < 0) {
			RhinoApp().Print(L"���Ȳ���С��0��������Ϊ0");
			Strength = 0;
		}
		else if (strength > 99) {
			RhinoApp().Print(L"���Ȳ��ô���99��������Ϊ99");
			Strength = 99;
		}
	}
	~ScaleZ() = default;
	HRESULT InnerEdit(ON_NurbsSurface* srf, ON_3dPoint* ptrCV, int* indices, const int& count) override;
};

class Flatten :public Operation {
public:
	Flatten(double targetheight) : Operation(targetheight) {
		if (targetheight < -99) {
			RhinoApp().Print(L"Ŀ��߶Ȳ���С��-99��������Ϊ-99");
			Strength = -99;
		}
		else if (targetheight > 99) {
			RhinoApp().Print(L"Ŀ��߶Ȳ��ô���99��������Ϊ99");
			Strength = 99;
		}
	}
	~Flatten() = default;
	HRESULT InnerEdit(ON_NurbsSurface* srf, ON_3dPoint* ptrCV, int* indices, const int& count) override;
};

class Shape {
public:
	double Offset;

	Shape(double offset) : Offset(offset) {
		if (offset < 0) {
			RhinoApp().Print(L"Offset����С��0��������Ϊ0");
			Offset = 0;
		}
		else if (offset > 99) {
			RhinoApp().Print(L"Offset���ô���99��������Ϊ99");
			Offset = 99;
		}
	}
	~Shape() = default;
	virtual HRESULT IsPointIn(const ON_3dPoint& pt, const ON_NurbsCurve* cv, bool& isPointInCurve) = 0;
	virtual HRESULT GetInnerAndOuterCurve(ON_NurbsCurve* innercurve, ON_NurbsCurve* outercurve, ON_3dPoint centerpoint) = 0;
};

class Circle :public Shape {
	double Radius;

public:
	Circle(double radius, double offset) :Radius(radius), Shape(offset) {
		if (radius < 0) {
			RhinoApp().Print(L"���ʰ뾶����С��0��������Ϊ0");
			Radius = 0.01;
		}
		else if (radius > 99) {
			RhinoApp().Print(L"���ʰ뾶���ô���99��������Ϊ99");
			Radius = 99;
		}
		else if (radius == 0) {
			Radius = 0.01;
		}

	}
	~Circle() = default;
	HRESULT IsPointIn(const ON_3dPoint& pt, const ON_NurbsCurve* cv, bool& isPointInCurve) override;
	HRESULT GetInnerAndOuterCurve(ON_NurbsCurve* innercurve, ON_NurbsCurve* outercurve, ON_3dPoint centerpoint) override;
};


class PolyGon :public Shape {
	double Radius;
	int PolyNum;

public:
	PolyGon(double radius, int polynum, double offset) :Radius(radius), PolyNum(polynum), Shape(offset) {
		if (radius < 0) {
			RhinoApp().Print(L"���ʰ뾶����С��0��������Ϊ0");
			Radius = 0.01;
		}
		else if (radius > 99) {
			RhinoApp().Print(L"���ʰ뾶���ô���99��������Ϊ99");
			Radius = 99;
		}
		else if (radius == 0) {
			Radius = 0.01;
		}

		if (polynum < 3) {
			RhinoApp().Print(L"������α�������С��3��������Ϊ3");
			PolyNum = 3;
		}
		else if (polynum > 12) {
			RhinoApp().Print(L"������α������ô���12��������Ϊ12");
			PolyNum = 12;
		}
	};
	~PolyGon() = default;
	HRESULT IsPointIn(const ON_3dPoint& pt, const ON_NurbsCurve* cv, bool& isPointInCurve) override;
	HRESULT GetInnerAndOuterCurve(ON_NurbsCurve* innercurve, ON_NurbsCurve* outercurve, ON_3dPoint centerpoint) override;
};

class RecTangle :public Shape {
	double Width;
	double Length;

public:
	RecTangle(double width, double length, double offset) :Width(width), Length(length), Shape(offset) {
		if (width < 0) {
			RhinoApp().Print(L"���ο�Ȳ���С��0��������Ϊ0");
			Width = 0.01;
		}
		else if (width > 99) {
			RhinoApp().Print(L"���ο�Ȳ��ô���99��������Ϊ99");
			Width = 99;
		}
		else if (width == 0) {
			Width = 0.01;
		}

		if (length < 0) {
			RhinoApp().Print(L"���γ��Ȳ���С��0��������Ϊ0");
			Length = 0.01;
		}
		else if (length > 99) {
			RhinoApp().Print(L"���γ��Ȳ��ô���99��������Ϊ99");
			Length = 99;
		}
		else if (Length == 0) {
			Length = 0.01;
		}

	};
	~RecTangle() = default;
	HRESULT IsPointIn(const ON_3dPoint& pt, const ON_NurbsCurve* cv, bool& isPointInCurve) override;
	HRESULT GetInnerAndOuterCurve(ON_NurbsCurve* innercurve, ON_NurbsCurve* outercurve, ON_3dPoint centerpoint) override;
};

class Customized :public Shape {
	ON_NurbsCurve* CustomizedCurve;

public:
	Customized(ON_NurbsCurve* customizedcurve, double offset) : CustomizedCurve(customizedcurve), Shape(offset) {};
	~Customized() = default;
	HRESULT IsPointIn(const ON_3dPoint& pt, const ON_NurbsCurve* cv, bool& isPointInCurve) override;
	HRESULT GetInnerAndOuterCurve(ON_NurbsCurve* innercurve, ON_NurbsCurve* outercurve, ON_3dPoint centerpoint) override;
};

class RhinoBrush {
public:
	Operation* pOperation;
	Shape* pShape;

	RhinoBrush(Operation* operation, Shape* shape) :pOperation(operation), pShape(shape) {};
	~RhinoBrush() = default;
};

// 0 Inflate ����
	// 00 InflateCircle Բ������
class InflateCircle : public RhinoBrush {
	Inflate operation;
	Circle shape;

public:
	InflateCircle(double offset, double strength, double radius) :operation(strength), shape(radius, offset), RhinoBrush(&operation, &shape) {};
	~InflateCircle() = default;
};


// 01 InflatePolygon �����������
class InflatePolygon : public RhinoBrush {
	Inflate operation;
	PolyGon shape;

public:
	InflatePolygon(double offset, double strength, double radius, int polynum) :operation(strength), shape(radius, polynum, offset), RhinoBrush(&operation, &shape) {};
	~InflatePolygon() = default;
};

// 02 InflateRectangle ��������
class InflateRectangle : public RhinoBrush {
	Inflate operation;
	RecTangle shape;

public:
	InflateRectangle(double offset, double strength, double width, double length) :operation(strength), shape(width, length, offset), RhinoBrush(&operation, &shape) {};
	~InflateRectangle() = default;
	virtual void OuterEdit() {};
};

// 03 InflateCustomized �Զ�����������
class InflateCustomized : public RhinoBrush {
	Inflate operation;
	Customized shape;

public:
	InflateCustomized(double offset, double strength, ON_NurbsCurve* customizedcurve) :operation(strength), shape(customizedcurve, offset), RhinoBrush(&operation, &shape) {};
	~InflateCustomized() = default;
};

// 1 ScaleZ ����
	// 10 ScaleZCircle Բ������
class ScaleZCircle : public RhinoBrush {
	ScaleZ operation;
	Circle shape;

public:
	ScaleZCircle(double offset, double strength, double radius) :operation(strength), shape(radius, offset), RhinoBrush(&operation, &shape) {};
	~ScaleZCircle() = default;
};

// 11 ScaleZPolygon �����������
class ScaleZPolygon : public RhinoBrush {
	ScaleZ operation;
	PolyGon shape;

public:
	ScaleZPolygon(double offset, double strength, double radius, int polynum) :operation(strength), shape(radius, polynum, offset), RhinoBrush(&operation, &shape) {};
	~ScaleZPolygon() = default;
};

// 12 ScaleZRectangle ��������
class ScaleZRectangle : public RhinoBrush {
	ScaleZ operation;
	RecTangle shape;

public:
	ScaleZRectangle(double offset, double strength, double width, double length) :operation(strength), shape(width, length, offset), RhinoBrush(&operation, &shape) {};
	~ScaleZRectangle() = default;
};

// 13 ScaleZCustomized �Զ�����������
class ScaleZCustomized : public RhinoBrush {
	ScaleZ operation;
	Customized shape;

public:
	ScaleZCustomized(double offset, double strength, ON_NurbsCurve* customizedcurve) :operation(strength), shape(customizedcurve, offset), RhinoBrush(&operation, &shape) {};
	~ScaleZCustomized() = default;
};

// 2 Flatten ѹƽ
	// 20 FlattenCircle Բ��ѹƽ
class FlattenCircle : public RhinoBrush {
	Flatten operation;
	Circle shape;

public:
	FlattenCircle(double offset, double targetheight, double radius) :operation(targetheight), shape(radius, offset), RhinoBrush(&operation, &shape) {};
	~FlattenCircle() = default;
};

// 21 FlattenPolygon �������ѹƽ
class FlattenPolygon : public RhinoBrush {
	Flatten operation;
	PolyGon shape;

public:
	FlattenPolygon(double offset, double targetheight, double radius, int polynum) :operation(targetheight), shape(radius, polynum, offset), RhinoBrush(&operation, &shape) {};
	~FlattenPolygon() = default;
};

// 22 FlattenRectangle ����ѹƽ
class FlattenRectangle : public RhinoBrush {
	Flatten operation;
	RecTangle shape;

public:
	FlattenRectangle(double offset, double targetheight, double width, double length) :operation(targetheight), shape(width, length, offset), RhinoBrush(&operation, &shape) {};
	~FlattenRectangle() = default;
};

// 23 FlattenCustomized �Զ�������ѹƽ
class FlattenCustomized : public RhinoBrush {
	Flatten operation;
	Customized shape;

public:
	FlattenCustomized(double offset, double targetheight, ON_NurbsCurve* customizedcurve) :operation(targetheight), shape(customizedcurve, offset), RhinoBrush(&operation, &shape) {};
	~FlattenCustomized() = default;
};

#endif