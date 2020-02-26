/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "geometryimplementationqt.h"

#include <QDebug>

GeometryImplementationQt::GeometryImplementationQt(VisualizerVTK *visualizerVTK,const SimTK::SimbodyMatterSubsystem& matter,const SimTK::State& state):SimTK::DecorativeGeometryImplementation(),m_state(state),m_matter(matter)
{

}

void GeometryImplementationQt::implementPointGeometry(const SimTK::DecorativePoint &geom)
{
    qDebug() << "loading qt geometry Point";
}

void GeometryImplementationQt::implementLineGeometry(const SimTK::DecorativeLine &geom)
{
    qDebug() << "loading qt geometry Line";
}

void GeometryImplementationQt::implementBrickGeometry(const SimTK::DecorativeBrick &geom)
{
    qDebug() << "loading qt geometry Brick";
}

void GeometryImplementationQt::implementCylinderGeometry(const SimTK::DecorativeCylinder &geom)
{
    qDebug() << "loading qt geometry Cylinder";
}

void GeometryImplementationQt::implementCircleGeometry(const SimTK::DecorativeCircle &geom)
{
    qDebug() << "loading qt geometry Cercle";
}

void GeometryImplementationQt::implementSphereGeometry(const SimTK::DecorativeSphere &geom)
{
    qDebug() << "loading qt geometry Sphere";
}

void GeometryImplementationQt::implementEllipsoidGeometry(const SimTK::DecorativeEllipsoid &geom)
{
    qDebug() << "loading qt geometry Ellipsoid";
}

void GeometryImplementationQt::implementFrameGeometry(const SimTK::DecorativeFrame &geom)
{
    qDebug() << "loading qt geometry Frame";
}

void GeometryImplementationQt::implementTextGeometry(const SimTK::DecorativeText &geom)
{
    qDebug() << "loading qt geometry text";
}

void GeometryImplementationQt::implementMeshGeometry(const SimTK::DecorativeMesh &geom)
{
    qDebug() << " from qt geometry no file ";
}

void GeometryImplementationQt::implementMeshFileGeometry(const SimTK::DecorativeMeshFile &geom)
{
    qDebug() << " from qt geometry "<< QString::fromStdString(geom.getMeshFile());
}

void GeometryImplementationQt::implementTorusGeometry(const SimTK::DecorativeTorus &geom)
{
    qDebug() << "loading qt geometry Torus";
}

void GeometryImplementationQt::implementArrowGeometry(const SimTK::DecorativeArrow &geom)
{
    qDebug() << "loading qt geometry Arrow";
}

void GeometryImplementationQt::implementConeGeometry(const SimTK::DecorativeCone &geom)
{
    qDebug() << "loading qt geometry Cone";
}

