#pragma once
#include "Texture.h"
#include "MathUtils.h"

namespace GE {
	class Particle
	{
	public:
		// Constructor
		Particle(Texture* t) {
			particleTex = t;

			x = y = z = 0.0f;
			baseX = baseY = baseZ = 0.0f;

			scaleX = scaleY = scaleZ = 0.1f;

			// Enables each particle to have a unique motion - prevents particles following a uniform circumference 2 * M_PI
			phase = randomPhase();
			amplitude = randomRange(0.05f, 0.2f);
			frequency = randomRange(0.3f, 1.2f);
		}

		~Particle() {

		}

		void update(float deltaTime) {
			// Update the current time
			currentTime += deltaTime;

			// Using a Simple Harmonic Motion (SHM) to animate the particles
			// position = basePosition + wave(time) * amplitude
			// Using sin for x and cos for y, it creates a orbital motion
			float currentX = baseX + sin(currentTime * frequency + phase) * amplitude;
			float currentY = baseY + cos(currentTime * frequency * 0.5f + phase) * amplitude;
			float currentZ = baseZ;

			setX(currentX);
			setY(currentY);
			setZ(currentZ);
		}

		// Generates a random "phase" value - offsets the position of the particle for random movement
		float randomPhase() {
			return static_cast<float>(rand()) / RAND_MAX * (2 * (M_PI));
		}

		// Accessor methods to allow access to position and scale values for rendering
		float getX() {
			return x;
		}
		float getY() {
			return y;
		}
		float getZ() {
			return z;
		}

		void initXYZ(float initX, float initY, float initZ) {
			baseX = x = initX;
			baseY = y = initY;
			baseZ = z = initZ;
		}

		void setXYZ(float newX, float newY, float newZ) {
			x = newX;
			y = newY;
			z = newZ;
		}

		void setX(float newX) {
			x = newX;
		}
		void setY(float newY) {
			y = newY;
		}
		void setZ(float newZ) {
			z = newZ;
		}

		// Return the texture
		Texture* getTexture() {
			return particleTex;
		}

		void setScaleX(float newX) {
			scaleX = newX;
		}
		void setScaleY(float newY) {
			scaleY = newY;
		}

		float getScaleX() {
			return scaleX;
		}
		float getScaleY() {
			return scaleY;
		}

	private:
		// Data
		// Position
		float x, y, z;
		float baseX, baseY, baseZ;

		// Scale
		float scaleX, scaleY, scaleZ;

		// Texture
		Texture* particleTex;

		// Phase shifts the starting angle of the motion (each particle starts at a different point)
		float phase = 0.0f;
		// Amplitude defines how far the particle moves from its original location (the height of the wave)
		float amplitude = 0.12f;
		// Frequency defines how fast the particle oscillates (the speed of the oscillation)
		float frequency = 0.5f;

		float currentTime = 0.0f;
	};
}
