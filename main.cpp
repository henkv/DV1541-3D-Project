#include <stdio.h>
#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>

#include "Window.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "LightManager.h"
#include "FullscreenQuad.h"
#include "DefferedRenderer.h"
#include "GlowEffect.h"
#include "FileLoader.h"

const size_t WINDOW_WIDTH = 800;
const size_t WINDOW_HEIGHT = 600;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
/*/
	try {
//*/
		Window window = { "DV1541 3D Project", WINDOW_WIDTH, WINDOW_HEIGHT };
		Camera camera = { {0, 0, -5}, {0,0,0}, {0,1,0} };
		Shader phongShader("shaders/Phong.vert", "shaders/Phong.frag");

		FullscreenQuad fullscreenQuad;
		GaussianBlur gaussianBlur(WINDOW_WIDTH, WINDOW_HEIGHT);

		GameObjectManager scene;

		Model manet = FileLoader::loadObj("models/manet.obj");
		Model floor = FileLoader::loadObj("models/floor_final.obj");
		Model sun = FileLoader::loadObj("models/sun.obj");
		Model planet = FileLoader::loadObj("models/planet.obj");

		scene.add(&manet);
		scene.add(&floor);
		scene.add(&sun);
		scene.add(&planet);

		floor.move({ 0, -3, 0 });

		LightManager lightManager;
		lightManager.add(LightManager::Light(vec3(-3, 0, 0), vec3(1, 0, 0)));
		lightManager.add(LightManager::Light(vec3(3, 0, -0), vec3(0, 1, 0)));

		DefferedRenderer defferedRenderer(WINDOW_WIDTH, WINDOW_HEIGHT);
		GlowEffect glowEffect(WINDOW_WIDTH, WINDOW_HEIGHT);

		float prevFrame = 0;
		float deltaTime = 0;
		while (window.isOpen()) {
			window.pollEvents();
			deltaTime = window.getTime() - prevFrame;
			prevFrame = window.getTime();

			planet.setPosition(vec3(cosf(prevFrame), 0, sinf(prevFrame)) * 2.5f);
			manet.setPosition(vec3(cosf(prevFrame), 0, -sinf(prevFrame)) * 5.0f);

			camera.update(deltaTime);
			
			scene.frontToBackSort(camera);

			defferedRenderer.renderScene(scene, lightManager, camera);
			glowEffect.renderGlow(defferedRenderer.getFinalTexture(), scene, camera);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			fullscreenQuad.drawTexture(glowEffect.getFinalTexture());

			window.swapBuffer();
		}
/*/
	}
	catch (const char * message) {
		printf("Error: %s", message);
		printf("\nPress enter to exit...");
		getchar();
	}
//*/

	return 0;
}