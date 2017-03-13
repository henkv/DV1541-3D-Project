#include <stdio.h>
#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>

#include "Window.h"
#include "Model.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "LightManager.h"
#include "FullscreenQuad.h"
#include "DefferedRenderer.h"
#include "GlowEffect.h"

const size_t WINDOW_WIDTH = 800;
const size_t WINDOW_HEIGHT = 600;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//*/
	try {
	//*/
		Window window = { "DV1541 3D Project", WINDOW_WIDTH, WINDOW_HEIGHT };
		Camera camera = { {0, 0, -5}, {0,0,0}, {0,1,0} };
		Shader phongShader("shaders/Phong.vert", "shaders/Phong.frag");

		FullscreenQuad fullscreenQuad;

		GameObjectManager objectManager;
		objectManager.add(&Model("models/manet.obj"));

		LightManager lightManager;
		lightManager.add(LightManager::Light( vec3(-10,   0, -10), vec3(  0, .5,  1) ));
		lightManager.add(LightManager::Light( vec3( 10, -10, -10), vec3(  0,  1, .5) ));
		lightManager.add(LightManager::Light( vec3(  0,  10,   0), vec3(  1,  1,  1) ));

		DefferedRenderer defferedRenderer(WINDOW_WIDTH, WINDOW_HEIGHT);
		GlowEffect glowEffect(WINDOW_WIDTH, WINDOW_HEIGHT);

		float prevFrame = window.getTime();
		float deltaTime = window.getTime();
		while (window.isOpen()) {
			window.pollEvents();
			deltaTime = window.getTime() - prevFrame;
			prevFrame = window.getTime();

			camera.update(deltaTime);
			defferedRenderer.renderScene(objectManager, lightManager, camera);
			glowEffect.renderGlow(defferedRenderer.getFinalTexture(), objectManager, camera);
			fullscreenQuad.drawTexture(glowEffect.getFinalTexture());

			window.swapBuffer();
		}
	//*/
	}
	catch (const char * message) {
		printf("Error: %s", message);
		printf("\nPress enter to exit...");
		getchar();
	}
	//*/

	return 0;
}