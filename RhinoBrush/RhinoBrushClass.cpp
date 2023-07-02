#include "StdAfx.h"
#include "RhinoBrushPlugIn.h"
#include "RhinoBrushClass.h"
#include <cmath>

///////////////////////////////////test
//HRESULT Circle::GetInnerAndOuterCurve(ON_NurbsCurve* innercurve, ON_NurbsCurve* outercurve, ON_3dPoint centerpoint) {
//	return S_OK;
//}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//����ʵ��


 //��START��
 //����������ֵ��ơ�
 //success: S_OK;   false:S_FALSE;

/////////////////////////////////////////////////////�ຯ��ʵ��

// Class Shape
// ��������IsPointInCurve��
// Circle
HRESULT Circle::GetInnerAndOuterCurve(ON_NurbsCurve* innercurve, ON_NurbsCurve* outercurve, ON_3dPoint centerpoint)
{
	ON_Circle innercircle(centerpoint, Radius);
	ON_Circle outercircle(centerpoint, Radius + Offset);
	innercircle.GetNurbForm(*innercurve);
	outercircle.GetNurbForm(*outercurve);
	return S_OK;
}

//Polygon
HRESULT PolyGon::GetInnerAndOuterCurve(ON_NurbsCurve* innercurve, ON_NurbsCurve* outercurve, ON_3dPoint centerpoint)
{
	const double pi = 3.1415926;

	ON_3dPointArray innerarray;
	for (int i = 0; i < PolyNum; i++)
	{
		double x = Radius * cos(2 * i * pi / PolyNum) + centerpoint.x;
		double y = Radius * sin(2 * i * pi / PolyNum) + centerpoint.y;
		innerarray.Append(ON_3dPoint(x, y, 0));
	}
	innerarray.Append(ON_3dPoint(Radius + centerpoint.x, centerpoint.y, 0));	//�պ�
	ON_PolylineCurve innerpolygon(innerarray);
	innerpolygon.GetNurbForm(*innercurve);

	ON_3dPointArray outerarray;
	double outerRadius = Radius + Offset / (cos(pi / PolyNum));
	for (int i = 0; i < PolyNum; i++)
	{
		double x = outerRadius * cos(2 * i * pi / PolyNum) + centerpoint.x;
		double y = outerRadius * sin(2 * i * pi / PolyNum) + centerpoint.y;
		outerarray.Append(ON_3dPoint(x, y, 0));
	}
	outerarray.Append(ON_3dPoint(outerRadius + centerpoint.x, centerpoint.y, 0));	//�պ�
	ON_PolylineCurve outerpolygon(outerarray);
	outerpolygon.GetNurbForm(*outercurve);

	return S_OK;
}

//Rectangle
HRESULT RecTangle::GetInnerAndOuterCurve(ON_NurbsCurve* innercurve, ON_NurbsCurve* outercurve, ON_3dPoint centerpoint)
{
	const double pi = 3.1415926;
	ON_3dPointArray innerarray;
	innerarray.Append(ON_3dPoint(Width / 2 + centerpoint.x, Length / 2 + centerpoint.y, 0));
	innerarray.Append(ON_3dPoint(-Width / 2 + centerpoint.x, Length / 2 + centerpoint.y, 0));
	innerarray.Append(ON_3dPoint(-Width / 2 + centerpoint.x, -Length / 2 + centerpoint.y, 0));
	innerarray.Append(ON_3dPoint(Width / 2 + centerpoint.x, -Length / 2 + centerpoint.y, 0));
	innerarray.Append(ON_3dPoint(Width / 2 + centerpoint.x, Length / 2 + centerpoint.y, 0));

	ON_PolylineCurve innerpolygon(innerarray);
	innerpolygon.GetNurbForm(*innercurve);

	ON_3dPointArray outerarray;
	outerarray.Append(ON_3dPoint(Width / 2 + Offset + centerpoint.x, Length / 2 + Offset + centerpoint.y, 0));
	outerarray.Append(ON_3dPoint(-Width / 2 - Offset + centerpoint.x, Length / 2 + Offset + centerpoint.y, 0));
	outerarray.Append(ON_3dPoint(-Width / 2 - Offset + centerpoint.x, -Length / 2 - Offset + centerpoint.y, 0));
	outerarray.Append(ON_3dPoint(Width / 2 + Offset + centerpoint.x, -Length / 2 - Offset + centerpoint.y, 0));
	outerarray.Append(ON_3dPoint(Width / 2 + Offset + centerpoint.x, Length / 2 + Offset + centerpoint.y, 0));

	ON_PolylineCurve outerpolygon(outerarray);
	outerpolygon.GetNurbForm(*outercurve);

	return S_OK;
}

//Customized
HRESULT Customized::GetInnerAndOuterCurve(ON_NurbsCurve* innercurve, ON_NurbsCurve* outercurve, ON_3dPoint centerpoint)
{
	//���㲽��ѹƽ����
	ON_NurbsCurve* FlattenedCurve = new ON_NurbsCurve;
	getZ_Planar(CustomizedCurve, FlattenedCurve);

	//��һ����ȡԭ���ߵ����ĵ�
	ON_3dPoint origCenter;
	if (getCenter(FlattenedCurve, origCenter) != S_OK)
	{
		return S_FALSE;
	}

	//�ڶ������ƶ�ԭ���ߵ�Ŀ��㣬���innercurve
	if (moveCurve(FlattenedCurve, innercurve, &centerpoint) != S_OK)
	{
		return S_FALSE;
	}

	delete FlattenedCurve;

	//��������offset�����outercurve
	if (offset_XY(innercurve, outercurve, Offset, centerpoint) != S_OK)
	{
		return S_FALSE;
	}

	return S_OK;
}

// ��������IsPointInCurve��
// Circle
HRESULT Circle::IsPointIn(const ON_3dPoint& pt, const ON_NurbsCurve* cv, bool& isPointInCurve)
{
	double dis = 0;
	double x = 0.5 * (cv->ControlPoint(0).x + cv->ControlPoint(4).x);
	double y = cv->ControlPoint(0).y;

	ON_3dPoint CenterPoint(x, y, 0);
	double r = 0.5 * (cv->ControlPoint(0).x - cv->ControlPoint(4).x);
	dis = pow((pow((pt.x - CenterPoint.x), 2) + pow((pt.y - CenterPoint.y), 2)), 0.5);

	if (dis <= r) {
		isPointInCurve = 1;
	}
	else isPointInCurve = 0;

	return S_OK;
}

// Polygon
HRESULT PolyGon::IsPointIn(const ON_3dPoint& pt, const ON_NurbsCurve* cv, bool& isPointInCurve) {
	double _theta = 0;
	double theta = 0;
	double alpha = 0;
	double dis = 0;
	double boundary = 0;
	const double PI = 3.1415926;
	long double x = 0;
	long double y = 0;
	double centerangle = (2 * PI / PolyNum);

	for (int i = 0; i < PolyNum; i++) {
		x += cv->ControlPoint(i).x;
		y += cv->ControlPoint(i).y;
	}
	x /= PolyNum;
	y /= PolyNum;
	ON_3dPoint CenterPoint(x, y, 0);
	double r = pow((pow((cv->ControlPoint(1).x - x), 2) + pow((cv->ControlPoint(1).y - y), 2)), 0.5);

	double target_x = pt.x - x;
	double target_y = pt.y - y;
	alpha = 0.5 * PI - PI / PolyNum;
	_theta = atan(target_y / target_x);

	if (target_x > 0) {
		theta = (_theta < 0) ? fmod(2 * PI + _theta, centerangle) : fmod(_theta, centerangle);
	}
	else if (target_x < 0) {
		theta = (_theta < 0) ? fmod(PI - _theta, centerangle) : fmod(PI - _theta, centerangle);
	}
	else if (target_x == 0 && target_y > 0) {
		theta = fmod(0.5 * PI, centerangle);
	}
	else if (target_x == 0 && target_y < 0) {
		theta = fmod(1.5 * PI, centerangle);
	}

	dis = pow((pow(target_x, 2) + pow(target_y, 2)), 0.5);
	boundary = r * sin(alpha) / sin(PI - alpha - theta);

	if (dis <= boundary)
	{
		isPointInCurve = 1;
	}
	else isPointInCurve = 0;

	return S_OK;
}

// Rectangle
HRESULT RecTangle::IsPointIn(const ON_3dPoint& pt, const ON_NurbsCurve* cv, bool& isPointInCurve) {
	double x = 0.5 * (cv->ControlPoint(1).x + cv->ControlPoint(3).x);
	double y = 0.5 * (cv->ControlPoint(1).y + cv->ControlPoint(3).y);
	ON_3dPoint CenterPoint(x, y, 0);
	double w = abs(cv->ControlPoint(1).x - cv->ControlPoint(3).x);
	double l = abs(cv->ControlPoint(1).y - cv->ControlPoint(3).y);

	//x,y�������ã����жϵ��������꣩
	x = pt.x - x;
	y = pt.y - y;
	if (abs(x) < 0.5 * w && abs(y) < 0.5 * l)
	{
		isPointInCurve = 1;
	}
	else isPointInCurve = 0;

	return S_OK;
}

// Customized
HRESULT Customized::IsPointIn(const ON_3dPoint& pt, const ON_NurbsCurve* cv, bool& isPointInCurve)
{
	//cv ��ˮƽ���պ����ߣ�

	if (!cv->IsClosed())
	{
		return S_FALSE;
	}


	//�߶���0�ĵ�
	ON_3dPoint ptZ(pt.x, pt.y, 0);
	//ȡ����㣬t��parameter of the CP
	double t;
	cv->GetClosestPoint(ptZ, &t);
	ON_3dPoint ptCP;
	cv->EvPoint(t, ptCP);

	//���߳���
	double cvLength;
	cv->GetLength(&cvLength);

	//����
	double vecCP_x = ptCP.x - ptZ.x;
	double vecCP_y = ptCP.y - ptZ.y;
	double CPdist = sqrt(vecCP_x * vecCP_x + vecCP_y * vecCP_y);
	//��λ����
	double unit_vec_x = vecCP_x / CPdist;
	double unit_vec_y = vecCP_y / CPdist;
	//�յ�����
	double vec_final_x = (CPdist + cvLength / 2) * unit_vec_x;
	double vec_final_y = (CPdist + cvLength / 2) * unit_vec_y;

	ON_3dPoint ptEND(ptZ.x + vec_final_x, ptZ.y + vec_final_y, 0);
	ON_LineCurve* line = new ON_LineCurve(ptZ, ptEND);
	//���-�յ㣬���㽻������
	ON_SimpleArray<ON_X_EVENT> x;
	line->IntersectCurve(cv, x);
	int intersect_count = x.Count();
	int count = 0;

	//�������>=0��������ͬ�����ϵĽ��㣬����
	for (int i = 0; i < intersect_count; i++)
	{
		ON_X_EVENT interEvent = x[i];
		ON_3dPoint interPt = interEvent.m_A[0];  //m_A[2] �ǽ���on first curve.
		// �������ж�����niuo
		double vector2_x = interPt.x - ptZ.x;
		double vector2_y = interPt.y - ptZ.y;
		if (unit_vec_x * vector2_x + unit_vec_y * vector2_y >= 0)
		{
			count++;
		}
	}

	delete line;
	//��������������-�������ڣ�ż������������
	if (count % 2 == 1)
	{
		isPointInCurve = true;
	}
	else isPointInCurve = false;

	return S_OK;
}

//��������Edit��
// Class Operation
// Inflate ������*���棬CV����������Ȧ�ڵ�index����������Ȧ�ڵ���������ˢǿ�ȣ�����ֵ����
HRESULT Inflate::InnerEdit(ON_NurbsSurface* srf, ON_3dPoint* ptrCV, int* indices, const int& count)
{
	for (int i = 0; i < count; i++)
	{
		int index = indices[i];
		ptrCV[index].z += Strength;
		int srf_U = srf->CVCount(0);
		int count_0 = index / srf_U;
		int count_1 = index % srf_U;
		if (srf->SetCV(count_0, count_1, ptrCV[index]) != true)
		{
			return S_FALSE;
		}
	}
	return S_OK;
}

// Flatten ѹƽ��*���棬CV����������Ȧ�ڵ�index����������Ȧ�ڵ�������Ŀ��߶�z��
HRESULT Flatten::InnerEdit(ON_NurbsSurface* srf, ON_3dPoint* ptrCV, int* indices, const int& count)
{
	int srf_U = srf->CVCount(0);

	for (int i = 0; i < count; i++)
	{
		int index = indices[i];
		ptrCV[index].z = Strength;

		int count_0 = index / srf_U;
		int count_1 = index % srf_U;

		if (srf->SetCV(count_0, count_1, ptrCV[index]) != true)
		{
			return S_FALSE;
		}
		srf->SetCV(count_0, count_1, ptrCV[index]);
	}
	return S_OK;
}

// ScaleZ 
HRESULT ScaleZ::InnerEdit(ON_NurbsSurface* srf, ON_3dPoint* ptrCV, int* indices, const int& count)
{
	//�õ���ߵ�z_max����͵�z_min,ƽ��ֵz_mean
	double z_max;
	double z_min;
	long double z_sum = 0;
	z_max = z_min = ptrCV[indices[0]].z;
	for (int i = 1; i < count; i++)
	{
		int index = indices[i];
		double& z_curr = ptrCV[index].z;
		if (z_curr > z_max)
		{
			z_max = z_curr;
		}
		else if (z_curr < z_min)
		{
			z_min = z_curr;
		}
		z_sum += z_curr;
	}
	int srf_U = srf->CVCount(0);
	//  ��mode=Mean �������е��ƽ���߶ȡ�
	if (mode == ScaleZ_mode::Mean)
	{
		double z_mean = z_sum / count;
		for (int i = 0; i < count; i++)
		{
			int index = indices[i];
			double& z_curr = ptrCV[index].z;
			double zmove;
			zmove = (z_mean - z_curr) * Strength;
			z_curr += zmove;
			int count_0 = index / srf_U;
			int count_1 = index % srf_U;
			if (srf->SetCV(count_0, count_1, ptrCV[index]) != true)
			{
				return S_FALSE;
			}
		}
		return S_OK;
	}

	//  ��mode=Mid ������͵�����ߵ��ƽ���߶ȡ�
	else
	{
		double z_mid = (z_max + z_min) / 2;
		for (int i = 0; i < count; i++)
		{
			int index = indices[i];
			double& z_curr = ptrCV[index].z;
			double zmove;
			zmove = (z_mid - z_curr) * Strength;
			z_curr += zmove;
			int count_0 = index / srf_U;
			int count_1 = index % srf_U;
			if (srf->SetCV(count_0, count_1, ptrCV[index]) != true)
			{
				return S_FALSE;
			}
		}
		return S_OK;
	}
}



/////////////////////////////////////////////// �������ܺ���

// ��ö�٣�ScaleZ���͡�
// Mean:�������е��ƽ���߶ȣ� Mid��������ߵ����͵�߶ȵ�ƽ����
//enum class ScaleZ_mode
//{
//	Mean = 0,
//	Mid = 1
//};


//���������޸�����Ϊͬ�߶�ƽ�����ߣ�����ø߶�z��
// [in]ON_Curve *cv:ԭʼ���� [out]ON_NurbsCurve* planarcv:�޸ĺ�ĵȸ�ƽ������
HRESULT getZ_Planar(const ON_Curve* cv, ON_NurbsCurve* planarcv)
{
	if (!cv->IsClosed())
	{
		//��ʾ��������ѡ��պ�����
		return S_FALSE;
	}
	cv->GetNurbForm(*planarcv);
	int cvCount = planarcv->CVCount();
	ON_3dPoint* pts = new ON_3dPoint[cvCount];

	for (int i = 0; i < cvCount; i++)
	{
		planarcv->GetCV(i, pts[i]);
		pts[i].z = 0;
		planarcv->SetCV(i, pts[i]);
	}

	delete[] pts;
	return S_OK;
}


//����������Ȧ��
// Ӧ�������б�ˢ��ģʽ��������Ȧ�е���inner curve�ľ����outer curve�ľ������z�仯����

	//��out_mode��
	//enum class Out_mode
	//{
	//	sin = 0,
	//	ligth_sin = 1
	//};

	//�߶���ͬ��������ά��ȡ����
inline double distXY(ON_3dPoint& a, ON_3dPoint& b)
{
	double dist;
	double xx = a.x - b.x;
	double yy = a.y - b.y;
	dist = sqrt(xx * xx + yy * yy);
	return dist;
}

// [in&out]surface* , [in] inner curve(0), [in] outer curve(0), [in&out] ���Ƶ�������, 
// [in] ��Ȧ�ڿ��Ƶ�����, [in] ��Ȧ�ڿ��Ƶ�����, [in] Out_mode �㷨
HRESULT OuterEdit(ON_NurbsSurface* srf, const ON_NurbsCurve* inner_cv, const ON_NurbsCurve* outer_cv, ON_3dPoint* ptrCV,
	int* indices_out, int* indices_in, const int& count_out, const int& count_in)
{
	//�߶�Ϊ0��inner points
	ON_3dPoint* innerpt0 = new ON_3dPoint[count_in];
	for (int i = 0; i < count_in; i++)
	{
		int index = indices_in[i];
		innerpt0[i] = ptrCV[index];
		innerpt0[i].z = 0;
	}

	//����������Ȧ�ڵ�
	for (int i = 0; i < count_out; i++)
	{
		int index_out = indices_out[i];
		double z_curr = ptrCV[index_out].z;

		int srf_U = srf->CVCount(0);
		int count_0 = index_out / srf_U;
		int count_1 = index_out % srf_U;

		//�߶���0��һ����Ȧ�ڵ�
		ON_3dPoint outerpt0 = ptrCV[index_out];
		outerpt0.z = 0;

		//������parameter ��t
		double tin, tout;
		inner_cv->GetClosestPoint(outerpt0, &tin);
		outer_cv->GetClosestPoint(outerpt0, &tout);

		//����t��������
		ON_3dPoint inner_CP;
		inner_CP = inner_cv->PointAt(tin);
		ON_3dPoint outer_CP;
		outer_CP = outer_cv->PointAt(tout);

		//����㵽�������ߵľ���
		double dist_in, dist_out;
		dist_in = distXY(outerpt0, inner_CP);
		dist_out = distXY(outerpt0, outer_CP);

		//����߶�Ӱ�캯����xֵ����sin������xֵ��
		const double pi = 3.1415926;
		double factor_x = dist_out / (dist_in + dist_out);
		double factor_y = (sin(pi * factor_x - 0.5 * pi) + 1) * 0.5;


		//������Ȧ�ڵ㣬�ҵ���outerpt0�����
		int min_index = indices_in[0];
		double min_dist = distXY(outerpt0, innerpt0[0]);

		for (int j = 1; j < count_in; j++)
		{
			//��¼��ǰj��Ӧ��indices
			int index_curr = indices_in[j];

			//��������õ��Ǹ߶�ȡ0������� innerpt0[j]
			double dist_curr = distXY(outerpt0, innerpt0[j]);
			if (dist_curr < min_dist)
			{
				min_dist = dist_curr;
				min_index = index_curr;
			}
		}

		//�����ĸ߶�
		double CP_z = ptrCV[min_index].z;

		//���������ƶ���z�������ƶ�
		double z_move = (CP_z - ptrCV[index_out].z) * factor_y;
		ptrCV[index_out].z += z_move;

		srf->SetCV(count_0, count_1, ptrCV[index_out]);
		//if (srf->SetCV(count_0, count_1, ptrCV[index_out]) != S_OK)
		//{
		//    return S_FALSE;
		//}
	}

	delete[] innerpt0;
	return S_OK;
}

//����������װ��OFFSET��
HRESULT offset_XY(const ON_NurbsCurve* inner_curve, ON_NurbsCurve* offseted, double& distance, ON_3dPoint& center)
{
	//ƽ�淨�������˴���Ϊ(0,0,1)
	ON_3dVector z(0, 0, 1);
	ON_SimpleArray<ON_Curve*> curve1;
	ON_SimpleArray<ON_Curve*> curve2;


	//����rhino�е�offsetָ����������һ�Σ�
	RhinoOffsetCurve(*inner_curve, distance, center, z, 3, 0.01, 0.01, true, 0, curve1);
	RhinoOffsetCurve(*inner_curve, -distance, center, z, 3, 0.01, 0.01, true, 0, curve2);

	//ȡ������offset���
	if (curve1.SizeOfArray() == 0) {
		curve2[0]->GetNurbForm(*offseted);
	}
	else if (curve2.SizeOfArray() == 0) {
		curve1[0]->GetNurbForm(*offseted);
	}
	else {
		double length1;
		curve1[0]->GetLength(&length1);
		double length2;
		curve2[0]->GetLength(&length2);
		(length1 > length2) ? curve1[0]->GetNurbForm(*offseted) : curve2[0]->GetNurbForm(*offseted);
	}

	return S_OK;
}

//��������getCenter��
HRESULT getCenter(const ON_NurbsCurve* curve, ON_3dPoint& center)
{
	if (!curve->IsPlanar())
	{
		return S_FALSE;
	}
	//ѡȡ���ߵı߿�box����ȡ���ĵ�
	ON_BoundingBox bbox;
	bbox = curve->BoundingBox();
	center = bbox.Center();
	return S_OK;
}

//��������2D_MoveCurve��
// [in] ON_NurbsCurve* curr_curve,[out] ON_NurbsCurve* new_curve [in]ON_3dPoint* Ŀ���
HRESULT moveCurve(const ON_NurbsCurve* curr_curve, ON_NurbsCurve* new_curve, const ON_3dPoint* to_center)
{
	ON_3dPoint* curr_center = new ON_3dPoint;
	if (getCenter(curr_curve, *curr_center) != S_OK)
	{
		return S_FALSE;
	}

	//vector from curr_center to to_center
	double vector_x = to_center->x - curr_center->x;
	double vector_y = to_center->y - curr_center->y;
	delete curr_center;

	// ȡ������curve�Ŀ��Ƶ�
	int cvCount = curr_curve->CVCount();
	ON_3dPoint* new_cvs = new ON_3dPoint[cvCount];

	//�������߽��в���������ԭ���ߣ��Ա��´�ʹ��
	*new_curve = *curr_curve;

	for (int i = 0; i < cvCount; i++)
	{
		curr_curve->GetCV(i, new_cvs[i]);
		new_cvs[i].x += vector_x;
		new_cvs[i].y += vector_y;

		//������Ӧ��curve���Ƶ�
		new_curve->SetCV(i, new_cvs[i]);
	}
	delete[] new_cvs;

	return S_OK;
}