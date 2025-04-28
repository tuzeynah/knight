#pragma once

#include <string>

#include "../../ext/jni/jni.h"
#include "../../ext/jni/jvmti.h"

enum minecraft_version {
	UNDEFINED,
	MAJOR_1_7_10,
	MAJOR_1_8_9
};

struct Java
{
	static void Init();
	static void Kill();

	static bool AssignClass(std::string name, jclass &out);
	static jclass findClass(JNIEnv* p_env, jvmtiEnv* p_tienv, const std::string& path);

	static void GetMinecraftVersion();

	static inline JNIEnv* Env;
	static inline jvmtiEnv* tiEnv;
	static inline bool Initialized;
	static inline minecraft_version Version;
};

