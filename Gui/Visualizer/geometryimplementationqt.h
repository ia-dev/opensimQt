/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef GEOMETRYIMPLEMENTATIONQT_H
#define GEOMETRYIMPLEMENTATIONQT_H

#include "visualizervtk.h"

#include <OpenSim.h>

class GeometryImplementationQt : public SimTK::DecorativeGeometryImplementation
{
public:
    GeometryImplementationQt(VisualizerVTK *visualizerVTK,const SimTK::SimbodyMatterSubsystem& matter,const SimTK::State& state);

    // DecorativeGeometryImplementation interface
public:
    virtual void implementPointGeometry(const SimTK::DecorativePoint &geom) override;
    virtual void implementLineGeometry(const SimTK::DecorativeLine  &geom) override;
    virtual void implementBrickGeometry(const SimTK::DecorativeBrick &geom) override;
    virtual void implementCylinderGeometry(const SimTK::DecorativeCylinder &geom) override;
    virtual void implementCircleGeometry(const SimTK::DecorativeCircle &geom) override;
    virtual void implementSphereGeometry(const SimTK::DecorativeSphere &geom) override;
    virtual void implementEllipsoidGeometry(const SimTK::DecorativeEllipsoid &geom) override;
    virtual void implementFrameGeometry(const SimTK::DecorativeFrame &geom) override;
    virtual void implementTextGeometry(const SimTK::DecorativeText &geom) override;
    virtual void implementMeshGeometry(const SimTK::DecorativeMesh &geom) override;
    virtual void implementMeshFileGeometry(const SimTK::DecorativeMeshFile &geom) override;
    virtual void implementTorusGeometry(const SimTK::DecorativeTorus &geom) override;
    virtual void implementArrowGeometry(const SimTK::DecorativeArrow &geom) override;
    virtual void implementConeGeometry(const SimTK::DecorativeCone &geom) override;

private:
    VisualizerVTK *m_visualizerVTK;
    const SimTK::State& m_state;
    const SimTK::SimbodyMatterSubsystem& m_matter;
};

#endif // GEOMETRYIMPLEMENTATIONQT_H
