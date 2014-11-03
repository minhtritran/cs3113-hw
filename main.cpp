app::Update(elapsed) {
	stageAnimationTime += elapsed;
}

void app::drawStageAnimationNoAnimation() {
	Draw
}

void app::drawStageAnimationLinear() {
	float animationValue;
	if(stageAnimationTime > 4.0) {
		animationValue = mapValue(stageAnimationTime, 5.0, 4.0, 0.0, 1.0);

	}
	Draw
}

void app::drawAnimation() {

}