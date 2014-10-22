class Matrix {
public:
	Matrix();

	union {
		float m[4][4];
		float m[16];
	}

	void identity();

	Matrix inverse();
	Matrix operator * (const Matrix &m2);
	Vector operator * (const Vector &v2);
};

class Vector {
public:
	Vector();
	Vector(float x, float y, float z);

	float length();
	void normalize();

	float x;
	float y;
	float z;
};

class Entity {
public:
	Matrix matrix;

	float x;
	float y;
	float scale_x;
	float scale_y;
	float rotation;
};

Entity::buildMatrix() {
	Matrix rotateMatrix;
	rotateMatrix.m[0][0] = 
}

App::fixedUpdate() {
	for (int i=0; i< entities.size(); i++) {
		if (!entities[i]->bullet && entities[i]->visible) {

		}
	}	
}

App::checkCollision(Entity* entity1, Entity* entity2) {
	if not same
		return false;
	entity1->buildMatrix();
	entity2->buildMatrix();

	Matrix entity1Inverse = entity1->matrix.inverse();
	Matrix entity2Inverse = entity2->matrix.inverse();

	Vector ent2TL = Vector(-entity2->width * 0.5, entity2->height * 0.5, 0.0);
	Vector ent2BL = Vector(-entity2->width * 0.5, -entity2->height * 0.5, 0.0);	
	...

	//transform to world coordinates
	ent2TL = entity2->matrix * ent2TL;
	ent2BL = entity2->matrix * ent2BL;
	...

	//check x axis
	float minX = std::min(std::min(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);
	//check y axis
	...

	//do the same for ent1

}	