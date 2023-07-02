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

/////////////////////////////////////////独立功能函数声明///////////////////////////////////////////////

//【函数：修改曲线为同高度平面曲线，并获得高度z】
// [in]ON_Curve *cv:原始曲线 [out]ON_NurbsCurve* planarcv:修改后的等高平面曲线
HRESULT getZ_Planar(const ON_Curve* cv, ON_NurbsCurve* planarcv);

//【函数：外圈】
// 应用于所有笔刷和模式，根据外圈中点与inner curve的距离和outer curve的距离决定z变化量。
	//【out_mode】
	//【枚举：外圈编辑曲线类型】
enum class Out_mode
{
	sin = 0,
	ligth_sin = 1
};
//【函数：高度相同的两个三维点取距离】
inline double distXY(ON_3dPoint& a, ON_3dPoint& b);
// [in&out]surface* , [in] inner curve(0), [in] outer curve(0), [in&out] 控制点数组名, 
// [in] 外圈内控制点数组, [in] 外圈内控制点数量, [in] Out_mode 算法
HRESULT OuterEdit(ON_NurbsSurface* srf, const ON_NurbsCurve* inner_cv, const ON_NurbsCurve* outer_cv, ON_3dPoint* ptrCV,
	int* indices_out, int* indices_in, const int& count_out, const int& count_in);

//【函数：封装的OFFSET】
HRESULT offset_XY(const ON_NurbsCurve* inner_curve, ON_NurbsCurve* offseted, double& distance, ON_3dPoint& center);

//【函数：getCenter】
HRESULT getCenter(const ON_NurbsCurve* curve, ON_3dPoint& center);

//【函数：MoveCurve】
// [in] ON_NurbsCurve* curr_curve,[out] ON_NurbsCurve* new_curve [in]ON_3dPoint* 目标点
HRESULT moveCurve(const ON_NurbsCurve* curr_curve, ON_NurbsCurve* new_curve, const ON_3dPoint* to_center);

//////////////////////////////////////////////类定义/////////////////////////////////////////////////////////

//Operation类及其三个子类
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
			RhinoApp().Print(L"力度不得小于-99，已设置为-99");
			Strength = -99;
		}
		else if (strength > 99) {
			RhinoApp().Print(L"力度不得大于99，已设置为99");
			Strength = 99;
		}
	}
	~Inflate() = default;
	HRESULT InnerEdit(ON_NurbsSurface* srf, ON_3dPoint* ptrCV, int* indices, const int& count) override;
};

// 【枚举：ScaleZ类型】
// Mean:趋于所有点的平均高度； Mid：趋于最高点和最低点高度的平均；
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
			RhinoApp().Print(L"力度不得小于0，已设置为0");
			Strength = 0;
		}
		else if (strength > 99) {
			RhinoApp().Print(L"力度不得大于99，已设置为99");
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
			RhinoApp().Print(L"目标高度不得小于-99，已设置为-99");
			Strength = -99;
		}
		else if (targetheight > 99) {
			RhinoApp().Print(L"目标高度不得大于99，已设置为99");
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
			RhinoApp().Print(L"Offset不得小于0，已设置为0");
			Offset = 0;
		}
		else if (offset > 99) {
			RhinoApp().Print(L"Offset不得大于99，已设置为99");
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
			RhinoApp().Print(L"画笔半径不得小于0，已设置为0");
			Radius = 0.01;
		}
		else if (radius > 99) {
			RhinoApp().Print(L"画笔半径不得大于99，已设置为99");
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
			RhinoApp().Print(L"画笔半径不得小于0，已设置为0");
			Radius = 0.01;
		}
		else if (radius > 99) {
			RhinoApp().Print(L"画笔半径不得大于99，已设置为99");
			Radius = 99;
		}
		else if (radius == 0) {
			Radius = 0.01;
		}

		if (polynum < 3) {
			RhinoApp().Print(L"正多边形边数不得小于3，已设置为3");
			PolyNum = 3;
		}
		else if (polynum > 12) {
			RhinoApp().Print(L"正多边形边数不得大于12，已设置为12");
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
			RhinoApp().Print(L"矩形宽度不得小于0，已设置为0");
			Width = 0.01;
		}
		else if (width > 99) {
			RhinoApp().Print(L"矩形宽度不得大于99，已设置为99");
			Width = 99;
		}
		else if (width == 0) {
			Width = 0.01;
		}

		if (length < 0) {
			RhinoApp().Print(L"矩形长度不得小于0，已设置为0");
			Length = 0.01;
		}
		else if (length > 99) {
			RhinoApp().Print(L"矩形长度不得大于99，已设置为99");
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

// 0 Inflate 增减
	// 00 InflateCircle 圆形增减
class InflateCircle : public RhinoBrush {
	Inflate operation;
	Circle shape;

public:
	InflateCircle(double offset, double strength, double radius) :operation(strength), shape(radius, offset), RhinoBrush(&operation, &shape) {};
	~InflateCircle() = default;
};


// 01 InflatePolygon 正多边形增减
class InflatePolygon : public RhinoBrush {
	Inflate operation;
	PolyGon shape;

public:
	InflatePolygon(double offset, double strength, double radius, int polynum) :operation(strength), shape(radius, polynum, offset), RhinoBrush(&operation, &shape) {};
	~InflatePolygon() = default;
};

// 02 InflateRectangle 矩形增减
class InflateRectangle : public RhinoBrush {
	Inflate operation;
	RecTangle shape;

public:
	InflateRectangle(double offset, double strength, double width, double length) :operation(strength), shape(width, length, offset), RhinoBrush(&operation, &shape) {};
	~InflateRectangle() = default;
	virtual void OuterEdit() {};
};

// 03 InflateCustomized 自定义曲线增减
class InflateCustomized : public RhinoBrush {
	Inflate operation;
	Customized shape;

public:
	InflateCustomized(double offset, double strength, ON_NurbsCurve* customizedcurve) :operation(strength), shape(customizedcurve, offset), RhinoBrush(&operation, &shape) {};
	~InflateCustomized() = default;
};

// 1 ScaleZ 缩放
	// 10 ScaleZCircle 圆形缩放
class ScaleZCircle : public RhinoBrush {
	ScaleZ operation;
	Circle shape;

public:
	ScaleZCircle(double offset, double strength, double radius) :operation(strength), shape(radius, offset), RhinoBrush(&operation, &shape) {};
	~ScaleZCircle() = default;
};

// 11 ScaleZPolygon 正多边形缩放
class ScaleZPolygon : public RhinoBrush {
	ScaleZ operation;
	PolyGon shape;

public:
	ScaleZPolygon(double offset, double strength, double radius, int polynum) :operation(strength), shape(radius, polynum, offset), RhinoBrush(&operation, &shape) {};
	~ScaleZPolygon() = default;
};

// 12 ScaleZRectangle 矩形缩放
class ScaleZRectangle : public RhinoBrush {
	ScaleZ operation;
	RecTangle shape;

public:
	ScaleZRectangle(double offset, double strength, double width, double length) :operation(strength), shape(width, length, offset), RhinoBrush(&operation, &shape) {};
	~ScaleZRectangle() = default;
};

// 13 ScaleZCustomized 自定义曲线缩放
class ScaleZCustomized : public RhinoBrush {
	ScaleZ operation;
	Customized shape;

public:
	ScaleZCustomized(double offset, double strength, ON_NurbsCurve* customizedcurve) :operation(strength), shape(customizedcurve, offset), RhinoBrush(&operation, &shape) {};
	~ScaleZCustomized() = default;
};

// 2 Flatten 压平
	// 20 FlattenCircle 圆形压平
class FlattenCircle : public RhinoBrush {
	Flatten operation;
	Circle shape;

public:
	FlattenCircle(double offset, double targetheight, double radius) :operation(targetheight), shape(radius, offset), RhinoBrush(&operation, &shape) {};
	~FlattenCircle() = default;
};

// 21 FlattenPolygon 正多边形压平
class FlattenPolygon : public RhinoBrush {
	Flatten operation;
	PolyGon shape;

public:
	FlattenPolygon(double offset, double targetheight, double radius, int polynum) :operation(targetheight), shape(radius, polynum, offset), RhinoBrush(&operation, &shape) {};
	~FlattenPolygon() = default;
};

// 22 FlattenRectangle 矩形压平
class FlattenRectangle : public RhinoBrush {
	Flatten operation;
	RecTangle shape;

public:
	FlattenRectangle(double offset, double targetheight, double width, double length) :operation(targetheight), shape(width, length, offset), RhinoBrush(&operation, &shape) {};
	~FlattenRectangle() = default;
};

// 23 FlattenCustomized 自定义曲线压平
class FlattenCustomized : public RhinoBrush {
	Flatten operation;
	Customized shape;

public:
	FlattenCustomized(double offset, double targetheight, ON_NurbsCurve* customizedcurve) :operation(targetheight), shape(customizedcurve, offset), RhinoBrush(&operation, &shape) {};
	~FlattenCustomized() = default;
};

#endif