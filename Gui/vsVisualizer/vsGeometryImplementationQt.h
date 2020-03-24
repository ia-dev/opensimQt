/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef vsGeometryImplementationQt_H
#define vsGeometryImplementationQt_H

#include "vsVisualizerVTK.h"

#include <OpenSim.h>

class vsGeometryImplementationQt : public SimTK::DecorativeGeometryImplementation
{
public:
    vsGeometryImplementationQt(vsVisualizerVTK *visualizerVTK,const SimTK::SimbodyMatterSubsystem& matter,const SimTK::State& state);

    SimTK::Transform calculateTransformForGeo(const SimTK::DecorativeGeometry &geo);
    void calculateScaleFactors(double *out,const SimTK::DecorativeGeometry &geo);

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


    OpenSim::Model *renderedModel() const;
    void setRenderedModel(OpenSim::Model *renderedModel);

private:
    vsVisualizerVTK *m_visualizerVTK;
    const SimTK::State& m_state;
    const SimTK::SimbodyMatterSubsystem& m_matter;
    OpenSim::Model *m_renderedModel;
};

#endif // vsGeometryImplementationQt_H
