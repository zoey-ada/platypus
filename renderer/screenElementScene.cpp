#include "screenElementScene.hpp"

#include <utilities/logging/logger.hpp>

void ScreenElementScene::onUpdate(Milliseconds delta)
{
	if (!Scene::onUpdate(delta))
		logWarning("update was not successful");
}
