/*
 * camera.cpp
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#include "camera.h"

#include "../debug/log.h"

using namespace Engine::Render;

bool Camera::perspective(float width, float height, float cutnear, float cutfar, float fov)
	{
	this->type=Type::PERSPECTIVE;
	this->fov=fov;
	this->scale=1.0f;
	this->width=width;
	this->height=height;
	this->cutnear=cutnear;
	this->cutfar=cutfar;

	updateProjectionMatrix();

	return true;
	}

bool Camera::ortho(float width, float height, float cutnear, float cutfar)
	{
	this->type=Type::ORTHOGRAPHIC;
	this->fov=0.0f;
	this->scale=1.0f;
	this->width=width;
	this->height=height;
	this->cutnear=cutnear;
	this->cutfar=cutfar;

	updateProjectionMatrix();

	return true;
	}

bool Camera::GUI(float width, float height)
	{
	this->type=Type::GUI;
	this->fov=0.0f;
	this->scale=1.0f;
	this->width=width;
	this->height=height;
	this->cutnear=-1.0f;
	this->cutfar=1.0f;

	updateProjectionMatrix();

	return true;
	}

void Camera::updateProjectionMatrix()
	{
	using namespace Math;

	const float N=cutnear;
	const float F=cutfar;

	switch(type)
		{
		case Type::ORTHOGRAPHIC:
			{
			const float TZ=-(F+N)/(F-N);

			this->projection=AMatrix(
					AVector(2.0f/width,           0,        0,  0),
					AVector(         0, 2.0f/height,        0,  0),
					AVector(         0,           0, -2/(F-N), TZ),
					AVector(         0,           0,        0,  1)
					);
			}
		break;

		case Type::PERSPECTIVE:
			{
			const float ASPECT=(float)width/height;
			const float FH=tan(fov/360.0*M_PI);
			const float FW=FH*ASPECT;
			const float C=-(F+N)/(F-N);
			const float D=-(2*F*N)/(F-N);

			this->projection=AMatrix(
					AVector(1/FW, 0, 0, 0),
					AVector(0, 1/FH, 0, 0),
					AVector(0, 0,  C, D),
					AVector(0, 0, -1, 0)
					);
			}
		break;

		default:
		case Type::GUI:
			{
			const float R=width;
			const float L=0;
			const float T=0;
			const float B=height;
			const float TX=-(R+L)/(R-L);
			const float TY=-(T+B)/(T-B);
			const float TZ=-(F+N)/(F-N);

			this->projection=AMatrix(
					AVector( 2/(R-L), 0, 0, TX),
					AVector(0,  2/(T-B), 0, TY),
					AVector(0, 0, -2/(F-N), TZ),
					AVector(0, 0, 0, 1)
					);
			}
		break;
		}
	}

void Camera::updateViewMatrix() const
	{
	using namespace Math;

	needupdate=false;

	if(type==Type::GUI)
		{
		this->view=AMatrix(
				AVector(1, 0, 0, 0),
				AVector(0, 1, 0, 0),
				AVector(0, 0, 1, 0),
				AVector(0, 0, 0, 1));
		}
	else
		{
		this->view=AMatrix(
				orientation.getRight(),
				orientation.getUp(),
				-orientation.getForward(),
				AVector(0, 0, 0, 1))*
			AMatrix(
				AVector(1, 0, 0, -orientation.getPosition().x),
				AVector(0, 1, 0, -orientation.getPosition().y),
				AVector(0, 0, 1, -orientation.getPosition().z),
				AVector(0, 0, 0, 1)
				);
		}
	}


Engine::Math::Orientation Camera::getBillboard(const Engine::Math::AVector& objpos) const
	{
	using namespace Math;

	switch(type)
		{
		case Type::ORTHOGRAPHIC:
			{
			const AVector f=AVectorNormalize(AVector(getForward().x, getForward().y, 0.0f));
			const float ce=AVectorDot(getForward(), f).w;

			if(ce<0.1f && ce>-0.1f)
				return Orientation(objpos, AVector(0, 0, 1), -f);//, AVector(1, 0.0f, 1));
			return Orientation(objpos, AVector(0, 0, 1), -f);//, AVector(1, 1.0f/ce, 1)); // TODO skala do orientacji
			}
		break;

		case Type::PERSPECTIVE:
			{
			const AVector dist=objpos-getPosition();
			const AVector f=AVectorNormalize(AVector(dist.x, dist.y, 0.0f));
			const AVector r=AVector(f.y, -f.x, 0.0f);

			return Orientation(objpos, r, AVector(0, 0, 1));//, AVector(1, 1, 1)); // TODO skala do orientacji
			}
		break;

		default:
		case Type::GUI:
			return Orientation::GUI;
		break;
		}
	}

Engine::Math::AVector Camera::getPointOnScreen(const Engine::Math::AVector& objpos) const
	{
	using namespace Math;

	AVector ret;

	ret=objpos-getPosition();
	ret=getView()*ret;
	ret=getProjection()*ret;
	if(type==Type::PERSPECTIVE)
		ret=ret/AVector(ret.z, ret.z, 1, 1);
	ret.x=(ret.x+1)*width/2;
	ret.y=(ret.y+1)*height/2;
	ret.z=0;
	ret.w=0;

	return ret;
	}

void Camera::getRay(int mx, int my, Engine::Math::AVector& raypos, Engine::Math::AVector& raydir) const
	{
	using namespace Math;

	float x;
	float y;

	if(type==Type::PERSPECTIVE)
		{
		const float aspect=(float)width/height;
		float fw, fh;
		fh=tan(fov/360.0*M_PI);
		fw=fh*aspect;

		x=( mx/width-0.5f)*2.0f;
		y=(-my/height+0.5f)*2.0f;

		x=fw*x;
		y=fh*y;

		raypos=getPosition() + getForward()*cutnear + getRight()*x + getUp()*y;
		raydir=AVectorNormalize(raypos-getPosition());
		}
	else
		{
		x=( mx/(float)width-0.5f)*width;
		y=(-my/(float)height+0.5f)*height;

		raypos=getPosition() - getForward()*cutnear + getRight()*x + getUp()*y;
		raydir=getForward();
		}
	}
