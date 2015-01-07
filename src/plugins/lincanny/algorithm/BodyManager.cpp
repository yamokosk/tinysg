#include "BodyManager.h"

BodyManager& BodyManager::getInstance()
{
	static BodyManager instance;

	return instance;
}

BodyManager::BodyManager(){
	initialized = 0;
	critical_count = body_count = 0;

	critical_bodies  = new RigidBody*[MAX_CRITICAL_BODIES];
	bodies = new RigidBody*[MAX_BODIES];

	distances = new double*[MAX_CRITICAL_BODIES];
	for(int i=0;i<MAX_CRITICAL_BODIES;i++){
		distances[i] = new double[MAX_BODIES];
	}

	critical_points = new VERTEX3D*[MAX_CRITICAL_BODIES];
	for(int i=0;i<MAX_CRITICAL_BODIES;i++){
		critical_points[i] = new VERTEX3D[MAX_BODIES];
	}

	points = new VERTEX3D*[MAX_CRITICAL_BODIES];
	for(int i=0;i<MAX_CRITICAL_BODIES;i++){
		points[i] = new VERTEX3D[MAX_BODIES];
	}

	critical_features = new FEATURE**[MAX_CRITICAL_BODIES];
	for(int i=0;i<MAX_CRITICAL_BODIES;i++){
		critical_features[i] = new FEATURE*[MAX_BODIES];
	}

	critical_groups = new FEATURE**[MAX_CRITICAL_BODIES];

	features = new FEATURE**[MAX_CRITICAL_BODIES];
	for(int i=0;i<MAX_CRITICAL_BODIES;i++){
		features[i] = new FEATURE*[MAX_BODIES];
	}

	body_groups = new FEATURE**[MAX_BODIES];

}

BodyManager::~BodyManager(){
	for(int i=0;i<body_count;i++){
		delete bodies[i];
	}

	for(int i=0;i<critical_count;i++){
		delete critical_bodies[i];
	}

	for(int i=0;i<critical_count;i++){
		delete [] distances[i];
		delete [] critical_points[i];
		delete [] points[i];
		delete [] critical_features[i];
		delete [] features[i];
	}
	delete [] critical_features;
	delete [] features;
	delete [] critical_points;
	delete [] points;
	delete [] distances;
	delete [] bodies;
	delete [] critical_bodies;
}

int BodyManager::addBody(BoundingMesh* mesh)
{
	if(body_count == MAX_BODIES){
		return -1;
	}
	bodies[body_count] = new RigidBody();
	bodies[body_count]->addBoundingMesh(mesh);
	body_groups[body_count] = new FEATURE*[bodies[body_count]->mesh_count];
	return body_count++;
}

int BodyManager::addBody(BoundingMesh** meshes, int mesh_count)
{
	if(body_count == MAX_BODIES){
		return -1;
	}
	bodies[body_count] = new RigidBody();
	for(int i=0;i<mesh_count;i++){
		//printf("add\n");
		bodies[body_count]->addBoundingMesh(meshes[i]);
	}
	body_groups[body_count] = new FEATURE*[bodies[body_count]->mesh_count];
	return body_count++;
}

int BodyManager::addBody(RigidBody* body)
{
	if(body_count == MAX_BODIES){
		return -1;
	}
	bodies[body_count] = body;
	body_groups[body_count] = new FEATURE*[bodies[body_count]->mesh_count];
	return body_count++;
}

int BodyManager::addCriticalBody(BoundingMesh* mesh){
	if(critical_count == MAX_CRITICAL_BODIES){
		return -1;
	}
	critical_bodies[critical_count] = new RigidBody();
	critical_bodies[critical_count]->addBoundingMesh(mesh);
	critical_groups[critical_count] = new FEATURE*[critical_bodies[critical_count]->mesh_count];
	return critical_count++;
}


int BodyManager::addCriticalBody(BoundingMesh** meshes, int mesh_count)
{
	if(critical_count == MAX_CRITICAL_BODIES){
		return -1;
	}
	critical_bodies[critical_count] = new RigidBody();
	for(int i=0;i<mesh_count;i++){
		//printf("add\n");
		critical_bodies[critical_count]->addBoundingMesh(meshes[i]);
	}
	critical_groups[critical_count] = new FEATURE*[critical_bodies[critical_count]->mesh_count];
	return critical_count++;
}

int BodyManager::addCriticalBody(RigidBody* body)
{
	if(critical_count == MAX_CRITICAL_BODIES){
		return -1;
	}
	critical_bodies[critical_count] = body;
	critical_groups[critical_count] = new FEATURE*[critical_bodies[critical_count]->mesh_count];
	return critical_count++;
}

void BodyManager::setCriticalTransform(int id, double x, double y, double z, double x_rot, double y_rot, double z_rot)
{
	if(id < critical_count && id > -1)
	{
		critical_bodies[id]->setTransform(x,y,z,x_rot,y_rot,z_rot);
	}
}


void BodyManager::setTransform(int id, double x, double y, double z, double x_rot, double y_rot, double z_rot)
{
	if(id < critical_count && id > -1)
	{
		bodies[id]->setTransform(x,y,z,x_rot,y_rot,z_rot);
	}
}

void BodyManager::setCriticalTransform(int id, const double* T)
{
	if(id < critical_count && id > -1)
	{
		critical_bodies[id]->setTransform(T);
	}
}

void BodyManager::setTransform(int id, const double* T)
{
	if(id < critical_count && id > -1)
	{
		bodies[id]->setTransform(T);
	}
}

double** BodyManager::getDistancesArray(){
	return distances;
}

FEATURE*** BodyManager::getCriticalFeatureArray(){
	return critical_features;
}

FEATURE*** BodyManager::getFeatureArray(){
	return features;
}

VERTEX3D** BodyManager::getCriticalPointArray(){
	return critical_points;
}

VERTEX3D** BodyManager::getPointArray(){
	return points;
}

RigidBody** BodyManager::getCriticalBodyArray(){
	return critical_bodies;
}

RigidBody** BodyManager::getBodyArray(){
	return bodies;
}

int BodyManager::getCriticalBodyCount(){
	return critical_count;
}

int BodyManager::getBodyCount(){
	return body_count;
}


bool BodyManager::init()
{
	for(int i=0;i<critical_count;i++)
	{
		for(int j=0;j<body_count;j++)
		{
			double dist;
			double min_dist = DOUBLE_MAX;
			VERTEX3D point1, point2;

			for(int m=0;m<critical_bodies[i]->mesh_count;m++)
			{
				for(int n=0;n<bodies[j]->mesh_count;n++)
				{
					dist = closestFeaturesInit(critical_bodies[i]->meshes[m]->getPoly(), bodies[j]->meshes[n]->getPoly(),
						                       critical_bodies[i]->transform, bodies[j]->transform, critical_groups[i][m], body_groups[j][n],
										       point1, point2, m);

					if(dist < min_dist)
					{
						min_dist = dist;
						critical_features[i][j] = critical_groups[i][n];
						features[i][j] = body_groups[j][n];
						critical_points[i][j] =  point1;
						points[i][j] = point2;
					}
				}
			}
			distances[i][j] = min_dist;
		}
	}
	initialized = 1;
	return true;
}

bool BodyManager::computeDistances()
{
	for(int i=0;i<critical_count;i++)
	{
		for(int j=0;j<body_count;j++)
		{
			double dist;
			double min_dist = DOUBLE_MAX;
			VERTEX3D point1, point2;

			for(int m=0;m<critical_bodies[i]->mesh_count;m++)
			{
				for(int n=0;n<bodies[j]->mesh_count;n++)
				{
					dist = closestFeatures(critical_bodies[i]->meshes[m]->getPoly(), bodies[j]->meshes[n]->getPoly(),
						                       critical_bodies[i]->transform, bodies[j]->transform, critical_groups[i][m], body_groups[j][n],
										       point1, point2, m, 50);

					if(dist < min_dist)
					{
						min_dist = dist;
						critical_features[i][j] = critical_groups[i][m];
						features[i][j] = body_groups[j][n];
						critical_points[i][j] = point1;
						points[i][j] = point2;
					}
				}
			}
			distances[i][j] = min_dist;
		}
	}
	initialized = 1;
	return true;
}
