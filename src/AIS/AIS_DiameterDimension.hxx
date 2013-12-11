// Copyright (c) 1995-1999 Matra Datavision
// Copyright (c) 1999-2012 OPEN CASCADE SAS
//
// The content of this file is subject to the Open CASCADE Technology Public
// License Version 6.5 (the "License"). You may not use the content of this file
// except in compliance with the License. Please obtain a copy of the License
// at http://www.opencascade.org and read it completely before using this file.
//
// The Initial Developer of the Original Code is Open CASCADE S.A.S., having its
// main offices at: 1, place des Freres Montgolfier, 78280 Guyancourt, France.
//
// The Original Code and all software distributed under the License is
// distributed on an "AS IS" basis, without warranty of any kind, and the
// Initial Developer hereby disclaims all such warranties, including without
// limitation, any warranties of merchantability, fitness for a particular
// purpose or non-infringement. Please see the License for the specific terms
// and conditions governing the rights and limitations under the License.

#ifndef _AIS_DiameterDimension_HeaderFile
#define _AIS_DiameterDimension_HeaderFile

#include <AIS.hxx>
#include <AIS_Dimension.hxx>
#include <gp_Pnt.hxx>
#include <gp_Circ.hxx>
#include <Standard.hxx>
#include <Standard_Macro.hxx>
#include <Standard_DefineHandle.hxx>

DEFINE_STANDARD_HANDLE (AIS_DiameterDimension, AIS_Dimension)

//! Diameter dimension. Can be constructued:
//! - On generic circle.
//! - On generic circle with user-defined anchor point on that circle
//!   (dimension plane is oriented to follow the anchor point).
//! - On generic circle in the specified plane.
//! - On generic shape containing geometry that can be measured
//!   by diameter dimension: circle wire, circular face, etc.
//! The anchor point is the location of the left attachement point of
//! dimension on the circle.
//! The anchor point computation is processed after dimension plane setting
//! so that positive flyout direction stands with normal of the circle and
//! the normal of the plane.
//! If the plane is user-defined the anchor point was computed as intersection
//! of the plane and the basis circle. Among two intersection points
//! the one is selected so that positive flyout direction vector and
//! the circle normal on the one side form the circle plane.
//! (corner between positive flyout directio nand the circle normal is acute.)
//! If the plane is computed automatically (by default it is the circle plane),
//! the anchor point is the zero parameter point of the circle.
//!
//! The dimension is considered as invalid if the user-defined plane
//! does not include th enachor point and th ecircle center,
//! if the diameter of the circle is less than Precision::Confusion().
//! In case if the dimension is built on the arbitrary shape, it can be considered
//! as invalid if the shape does not contain circle geometry.
//!
class AIS_DiameterDimension : public AIS_Dimension
{
public:

  //! Construct diameter dimension for the circle.
  //! @param theCircle [in] the circle to measure.
  Standard_EXPORT AIS_DiameterDimension (const gp_Circ& theCircle);

  //! Construct diameter dimension for the circle and orient it correspondingly
  //! to the passed plane.
  //! @param theCircle [in] the circle to measure.
  //! @param thePlane [in] the plane defining preferred orientation
  //!        for dimension.
  Standard_EXPORT AIS_DiameterDimension (const gp_Circ& theCircle,
                                         const gp_Pln& thePlane);

  //! Construct diameter on the passed shape, if applicable.
  //! @param theShape [in] the shape to measure.
  Standard_EXPORT AIS_DiameterDimension (const TopoDS_Shape& theShape);

  //! Construct diameter on the passed shape, if applicable - and
  //! define the preferred plane to orient the dimension.
  //! @param theShape [in] the shape to measure.
  //! @param thePlane [in] the plane defining preferred orientation
  //!        for dimension.
  Standard_EXPORT AIS_DiameterDimension (const TopoDS_Shape& theShape,
                                         const gp_Pln& thePlane);

public:

  //! @return measured geometry circle.
  const gp_Circ& Circle() const
  {
    return myCircle;
  }

  //! @return anchor point on circle for diameter dimension.
  Standard_EXPORT gp_Pnt AnchorPoint();

  //! @return the measured shape.
  const TopoDS_Shape& Shape() const
  {
    return myShape;
  }

public:

  //! Measure diameter of the circle.
  //! The actual dimension plane is used for determining anchor points
  //! on the circle to attach the dimension lines to.
  //! The dimension will become invalid if the diameter of the circle
  //! is less than Precision::Confusion().
  //! @param theCircle [in] the circle to measure.
  Standard_EXPORT void SetMeasuredGeometry (const gp_Circ& theCircle);

  //! Measure diameter on the passed shape, if applicable.
  //! The dimension will become invalid if the passed shape is not
  //! measurable or if measured diameter value is less than Precision::Confusion().
  //! @param theShape [in] the shape to measure.
  Standard_EXPORT void SetMeasuredGeometry (const TopoDS_Shape& theShape);

  //! @return the display units string.
  Standard_EXPORT virtual const TCollection_AsciiString& GetDisplayUnits () const;
  
  //! @return the model units string.
  Standard_EXPORT virtual const TCollection_AsciiString& GetModelUnits () const;

  Standard_EXPORT virtual void SetDisplayUnits (const TCollection_AsciiString& theUnits);

  Standard_EXPORT virtual void SetModelUnits (const TCollection_AsciiString& theUnits);

public:

  DEFINE_STANDARD_RTTI(AIS_DiameterDimension)

protected:

  //! Override this method to change logic of anchor point computation.
  //! Computes anchor point. Its computation is based on the current
  //! dimension plane. Therfore, anchor point is an intersection of plane
  //! and circle.
  //! ATTENTION!
  //! 1) The plane should be set or computed before.
  //! 2) The plane should inclide th ecircle center to be valid.
  Standard_EXPORT virtual void ComputeAnchorPoint();

  Standard_EXPORT virtual void ComputePlane();

  //! Checks if the center of the circle is on the plane.
  Standard_EXPORT virtual Standard_Boolean CheckPlane (const gp_Pln& thePlane) const;

  Standard_EXPORT virtual Standard_Real ComputeValue() const;

  Standard_EXPORT virtual void Compute (const Handle(PrsMgr_PresentationManager3d)& thePresentationManager,
                                        const Handle(Prs3d_Presentation)& thePresentation,
                                        const Standard_Integer theMode = 0);

  Standard_EXPORT virtual void ComputeFlyoutSelection (const Handle(SelectMgr_Selection)& theSelection,
                                                       const Handle(SelectMgr_EntityOwner)& theEntityOwner);

protected:

  //! Compute points on the circle sides for the specified dimension plane.
  //! Program error exception is raised if the dimension plane "x" direction 
  //! is orthogonal to plane (the "impossible" case). The passed dimension plane
  //! is the one specially computed to locate dimension presentation in circle.
  //! @param theCircle [in] the circle.
  //! @param thePlane [in] the dimension presentation plane computed.
  //! @param theFirstPnt [out] the first point.
  //! @param theSecondPnt [out] the second point.
  Standard_EXPORT void ComputeSidePoints (const gp_Circ& theCircle,
                                          const gp_Pln& thePlane,
                                          gp_Pnt& theFirstPnt,
                                          gp_Pnt& theSecondPnt);

  Standard_EXPORT Standard_Boolean IsValidCircle (const gp_Circ& theCircle) const;

  Standard_EXPORT Standard_Boolean IsValidAnchor (const gp_Circ& theCircle,
                                                  const gp_Pnt& thePnt) const;

private:

  gp_Circ          myCircle;
  gp_Pnt           myAnchorPoint;
  TopoDS_Shape     myShape;
};

#endif // _AIS_DiameterDimension_HeaderFile
