#pragma once
#include "Maths.h"

namespace dae
{
	namespace BRDF
	{
		/**
		 * \param kd Diffuse Reflection Coefficient
		 * \param cd Diffuse Color
		 * \return Lambert Diffuse Color
		 */
		static ColorRGB Lambert(float kd, const ColorRGB& cd)
		{
			const ColorRGB Lambert = kd * cd / PI;
			return { Lambert };
		}

		static ColorRGB Lambert(const ColorRGB& kd, const ColorRGB& cd)
		{
			const ColorRGB Lambert = kd * cd / PI;
			return { Lambert };
		}

		/**
		 * \brief todo
		 * \param ks Specular Reflection Coefficient
		 * \param exp Phong Exponent
		 * \param l Incoming (incident) Light Direction
		 * \param v View Direction
		 * \param n Normal of the Surface
		 * \return Phong Specular Color
		 */
		static ColorRGB Phong(float ks, float exp, const Vector3& l, const Vector3& v, const Vector3& n)
		{
			//const Vector3 reflect{ l - 2 * (Vector3::Dot(n,l) * n) };
			const float cosAngle{ Vector3::Dot(Vector3::Reflect(n,l) , v)};
			const float phong{ ks * powf(abs(cosAngle),exp) };
			return {ColorRGB(phong,phong,phong)};
		}

		/**
		 * \brief BRDF Fresnel Function >> Schlick
		 * \param h Normalized Halfvector between View and Light directions
		 * \param v Normalized View direction
		 * \param f0 Base reflectivity of a surface based on IOR (Indices Of Refrection), this is different for Dielectrics (Non-Metal) and Conductors (Metal)
		 * \return
		 */
		static ColorRGB FresnelFunction_Schlick(const Vector3& h, const Vector3& v, const ColorRGB& f0)
		{
			ColorRGB schilck{ f0 + (ColorRGB(1.f,1.f,1.f) - f0) * powf(1.f - Vector3::Dot(h,v),5.f) };
			return { schilck };
		}

		/**
		 * \brief BRDF NormalDistribution >> Trowbridge-Reitz GGX (UE4 implemetation - squared(roughness))
		 * \param n Surface normal
		 * \param h Normalized half vector
		 * \param roughness Roughness of the material
		 * \return BRDF Normal Distribution Term using Trowbridge-Reitz GGX
		 */
		static float NormalDistribution_GGX(const Vector3& n, const Vector3& h, float roughness)
		{
			float alpha{ powf(roughness,4)};
			float normalDistr{ alpha / (PI * powf(Vector3::Dot(n,h) * Vector3::Dot(n,h) * (alpha - 1) + 1,2)) };
			
			return {normalDistr};
		}


		/**
		 * \brief BRDF Geometry Function >> Schlick GGX (Direct Lighting + UE4 implementation - squared(roughness))
		 * \param n Normal of the surface
		 * \param v Normalized view direction
		 * \param roughness Roughness of the material
		 * \return BRDF Geometry Term using SchlickGGX
		 */
		static float GeometryFunction_SchlickGGX(const Vector3& n, const Vector3& v, float roughness)
		{
			float alpha{ powf(roughness,2) };
			float kdirect{ (alpha*alpha + 1)*(alpha*alpha+1) / 8 };
			float SchlickGGX{ Vector3::Dot(n,v) / (Vector3::Dot(n,v) * (1 - kdirect) + kdirect) };
			return {SchlickGGX};
		}

		/**
		 * \brief BRDF Geometry Function >> Smith (Direct Lighting)
		 * \param n Normal of the surface
		 * \param v Normalized view direction
		 * \param l Normalized light direction
		 * \param roughness Roughness of the material
		 * \return BRDF Geometry Term using Smith (> SchlickGGX(n,v,roughness) * SchlickGGX(n,l,roughness))
		 */
		static float GeometryFunction_Smith(const Vector3& n, const Vector3& v, const Vector3& l, float roughness)
		{
			
			return { BRDF::GeometryFunction_SchlickGGX(n, v, roughness) * BRDF::GeometryFunction_SchlickGGX(n, l, roughness) };
		}

	}
}