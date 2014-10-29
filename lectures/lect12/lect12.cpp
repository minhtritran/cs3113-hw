checkCollision(entity1, entity2, vector& adjust) {
	adjust.x = 0.0;
	adjust.y = 0.0;
}

testSeparationForEdge(edgeX, edgeY, const vector& points1, const vector& points2, vector& penetration)

//////////////////////////////////////////

GLuint levelVertexBuffer
gLuit levelUVBuffer

//in buildLevel
glGenBuffer(1, &levelVertexBuffer);
glBindBuffer levelVertexBuffer
glBufferData vertexData.data()

glGenBuffer(1, &levelUVBuffer)
glBindBuffer levelUVBuffer
glBufferData texCoorddata.data()

numlevelvertices = numvertices;

//in renderlevel
glbindtexture spritesheettexture
glenable texture2d
glenable blend
glblendfunc

glenableclientstate glvertexarray
glenableclientstate gltexturecoordarray

glBindBuffer levelVertexBuffer
glvertexpointer null
glbindbuffer 0

glbindbuffer levelUVBuffer
gltexcoordpointer null
glbindbufer 0

gldrawarrays numlevelvertices
