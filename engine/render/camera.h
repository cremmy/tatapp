/*
 * camera.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include "../math/orientation.h"

namespace Engine
	{
	namespace Render
		{
		class Camera
			{
			public:
				enum class Type
					{
					PERSPECTIVE,
					ORTHOGRAPHIC,
					GUI,
					POSTPROCESS,
					};

			protected:
				Math::Orientation orientation;
				Type type;

				float fov;
				float scale;

				float width;
				float height;

				float cutnear;
				float cutfar;

				Math::AMatrix projection;
				mutable Math::AMatrix view;

				mutable bool needupdate;

				void updateProjectionMatrix();
				void updateViewMatrix() const;

				//bool intposition;

			public:
				Camera(): orientation(), type(Type::ORTHOGRAPHIC), fov(60.0f), scale(1.0f), width(1.0f), height(1.0f), cutnear(0.0f), cutfar(1000.0f), needupdate(true) {/*updateProjectionMatrix(); updateViewMatrix();*/}
				~Camera() {}

				bool perspective(float width, float height, float cutnear=1.0, float cutfar=1000.0f, float fov=60.0f);
				bool ortho(float width, float height, float cutnear=0.0, float cutfar=1000.0f);
				bool GUI(float width, float height);
				bool postprocess();

				Math::AVector getPosition() const {return orientation.getPosition();}
				Math::AVector getRight() const {return orientation.getRight();}
				Math::AVector getForward() const {return orientation.getForward();}
				Math::AVector getUp() const {return orientation.getUp();}
				//Math::AQuaternion getRotation() const {return orientation.getRotation();}

				const Math::AMatrix& getProjection() const {return projection;}
				//const Math::AMatrix& getView() const {return view;}
				const Math::AMatrix& getView() const {if(needupdate) updateViewMatrix(); return view;}

				Math::Orientation getBillboard(const Math::AVector& pos) const;
				Math::AVector getPointOnScreen(const Math::AVector& pos) const;
				void getRay(int mx, int my, Math::AVector& raypos, Math::AVector& raydir) const;

				void setScale(float s) {scale=s; updateProjectionMatrix();}
				void setFOV(float s) {fov=s; updateProjectionMatrix();}

				void setPosition(const Math::AVector& pos) {orientation.setPosition(pos); needupdate=true;}
				void setOrientation(const Math::Orientation& ori) {orientation=ori; needupdate=true;}
				void move(const Math::AVector& path) {orientation.move(path); needupdate=true;}
				void moveRight(float dist) {orientation.moveRight(dist); needupdate=true;}
				void moveForward(float dist) {orientation.moveForward(dist); needupdate=true;}
				void moveUp(float dist) {orientation.moveUp(dist); needupdate=true;}
				void rotateRight(float angle) {orientation.rotateRight(angle); needupdate=true;}
				void rotateForward(float angle) {orientation.rotateForward(angle); needupdate=true;}
				void rotateUp(float angle) {orientation.rotateUp(angle); needupdate=true;}
				void rotateX(float angle) {orientation.rotateX(angle); needupdate=true;}
				void rotateY(float angle) {orientation.rotateY(angle); needupdate=true;}
				void rotateZ(float angle) {orientation.rotateZ(angle); needupdate=true;}
				void rotate(const Math::AVector& axis, float angle) {orientation.rotate(axis, angle); needupdate=true;}

				void lookAt(const Math::AVector& target, const Math::AVector& from, const Math::AVector& up=Math::AVector(0, 0, 1)) {orientation.lookAt(target, from, up); updateViewMatrix();}
				void lookAt(const Math::AVector& target, float angle, float elevation, float distance, float roll=0.0f) {orientation.lookAt(target, angle, elevation, distance, roll); updateViewMatrix();}

				/*void movePathHermite(const Math::AVector& from, const Math::AVector& handlea, const Math::AVector& target, const Math::AVector& hangleb, float t);
				void movePathBezier(const Math::AVector& from, const Math::AVector& handlea, const Math::AVector& target, const Math::AVector& hangleb, float t);
				void movePathBSpline(const Math::AVector& from, const Math::AVector& handlea, const Math::AVector& target, const Math::AVector& hangleb, float t);
				void movePathNURBS(const Math::AVector& from, const Math::AVector& handlea, const Math::AVector& target, const Math::AVector& hangleb, float t);*/
			};

		} /* namespace Graphics */
	} /* namespace Engine */
