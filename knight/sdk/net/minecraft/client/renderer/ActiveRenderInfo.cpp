#include "ActiveRenderInfo.h"

#include "../../../../sdk.h"
#include "../../../../java/nio/FloatBuffer.h"

CActiveRenderInfo::CActiveRenderInfo()
{
	this->Class = StrayCache::activeRenderInfo_class;
	this->FieldIDs["PROJECTION"] = StrayCache::activeRenderInfo_Projection;
	this->FieldIDs["MODELVIEW"] = StrayCache::activeRenderInfo_ModelView;
}

jclass CActiveRenderInfo::GetClass()
{
	return this->Class;
}

jobject CActiveRenderInfo::GetInstance()
{
	return NULL;
}

Matrix CActiveRenderInfo::ProjectionMatrix()
{
	jobject projection = Java::Env->GetStaticObjectField(this->GetClass(), this->FieldIDs["PROJECTION"]);
	Matrix m = FloatBuffer::FloatBuffer(projection).GetMatrix();
	Java::Env->DeleteLocalRef(projection);
	return m;
}

Matrix CActiveRenderInfo::ModelViewMatrix()
{
	jobject modelView = Java::Env->GetStaticObjectField(this->GetClass(), this->FieldIDs["MODELVIEW"]);
	Matrix m = FloatBuffer::FloatBuffer(modelView).GetMatrix();
	Java::Env->DeleteLocalRef(modelView);
	return m;
}

