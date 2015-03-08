#ifndef MAT4_H
#define MAT4_H

#include <iostream>
#include <string>
#include <array>
#include "vec3.h"

namespace math
{

	struct mat4
	{
	public:
		mat4()
		{
			clear();
		}

		mat4 clear()
		{
			m.fill(0.0f);

			return *this;
		}

		mat4 initIdentity()
		{
			clear();

			m[0 + 0 * 4] = 1;
			m[1 + 1 * 4] = 1;
			m[2 + 2 * 4] = 1;
			m[3 + 3 * 4] = 1;

			return *this;
		}

		mat4 initTranslation(float x, float y, float z)
		{
			this->initIdentity();

			m[0 + 3 * 4] = x;
			m[1 + 3 * 4] = y;
			m[2 + 3 * 4] = z;

			return *this;
		}

		mat4 initRotation(float w, float x, float y, float z)
		{
			return *this; // <-- I'm too lazy atm to do this now, whatever
		}

		mat4 initScale(float x, float y, float z)
		{
			this->initIdentity();

			m[0 + 0 * 4] = x;
			m[1 + 1 * 4] = y;
			m[2 + 2 * 4] = z;

			return *this;
		}

		mat4 operator*(const mat4 &other)
		{
			mat4 data = mat4();

			for (unsigned int j = 0; j < 4; j++)
			{
				for (unsigned int i = 0; i < 4; i++)
				{
					data.m[i + j * 4] = m[0 + j * 4] * other.m[i + 0 * 4] +
										m[0 + j * 4] * other.m[i + 0 * 4] +
										m[0 + j * 4] * other.m[i + 0 * 4] +
										m[0 + j * 4] * other.m[i + 0 * 4];
				}
			}

			return data;
		}

		float getValueAtIndex(int x, int y) const
		{
			unsigned int index = x + y * 4;

			if (index > 15)
			{
				std::cerr << "mat4.h ERROR: Trying to access 4x4 matrix index that's out of bounds! Column: " << x << " Row: " << y << "\n";
				return 0.0f;
			}

			return m[index];
		}
	private:
		std::array<float, 16> m;
	};

}

#endif