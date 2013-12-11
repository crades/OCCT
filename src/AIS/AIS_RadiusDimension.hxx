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

#ifndef _AIS_RadiusDimension_HeaderFile
#define _AIS_RadiusDimension_HeaderFile

#include <AIS.hxx>
#include <AIS_Dimension.hxx>
#include <gp_Pnt.hxx>
#include <gp_Circ.hxx>
#include <Standard.hxx>
#include <Standard_Macro.hxx>

DEFINE_STANDARD_HANDLE (AIS_RadiusDimension,AIS_Dimension)

//! Radius dimension. Can be constructued:
//! - On generic circle.
//! - On generic circle with user-defined anchor point on that circle.
//! - On generic shape containing geometry that can be measured
//!   by diameter dimension: circle wire, arc, circular face, etc.
//! The anchor point is the location of left attachement point of
//! dimension on the circle. It can be user-specified, or computed as
//! middle point on the arc. The radius dimension always lies in the
//! plane of the measured circle. The dimension is considered as
//! invalid if the user-specified anchor point is not lying on the circle,
//! if the radius of the circle is less than Precision::Confusion().
//! In case if the dimension is built on the arbitrary shape,
//! it can be considered as invalid if the shape does not contain
//! circle geometry.
class AIS_RadiusDimension : public AIS_Dimension
{
public:

  //! Create radius dimension for the circle geometry.
  //! @param theCircle [in] the circle to measure.
  Standard_EXPORT AIS_RadiusDimension (const gp_Circ& theCircle);

  //! Create radius dimension for the circle geometry and define its
  //! orientation by location of the first point on that circle.
  //! @param theCircle [in] the circle to measure.
  //! @param theAnchorPoint [in] the point to define the position
  //!        of the dimension attachement on the circle.
  Standard_EXPORT AIS_RadiusDimension (const gp_Circ& theCircle,
                                       const gp_Pnt& theAnchorPoint);

  //! Create radius dimension for the arbitrary shape (if possible).
  //! @param theShape [in] the shape to measure.
  Standard_EXPORT AIS_RadiusDimension (const TopoDS_Shape& theShape);

public:

  //! @return measured geometry circle.
  const gp_Circ& Circle() const
  {
    return myCircle;
  }

  //! @return anchor point on circle for radius dimension.
  const gp_Pnt& AnchorPoint() const
  {
    return myAnchorPoint;
  }

  //! @return the measured shape.
  const TopoDS_Shape& Shape() const
  {
    return myShape;
  }

public:

  //! Measure radius of the circle.
  //! The dimension will become invalid if the radius of the circle
  //! is less than Precision::Confusion().
  //! @param theCircle [in] the circle to measure.
  Standard_EXPORT void SetMeasuredGeometry (const gp_Circ& theCircle);

  //! Measure radius of the circle and orient the dimension so
  //! the dimension lines attaches to anchor point on the circle.
  //! The dimension will become invalid if the radiuss of the circle
  //! is less than Precision::Confusion().
  //! @param theCircle [in] the circle to measure.
  //! @param theAnchorPoint [in] the point to attach the dimension lines.
  Standard_EXPORT void SetMeasuredGeometry (const gp_Circ& theCircle,
                                            const gp_Pnt& theAnchorPoint);

  //! Measure radius on the passed shape, if applicable.
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

  DEFINE_STANDARD_RTTI (AIS_RadiusDimension)

protected:

  Standard_EXPORT virtual void ComputePlane();

  //! Checks if anchor point and the center of the circle are on the plane.
  Standard_EXPORT virtual Standard_Boolean CheckPlane (const gp_Pln& thePlane) const;

  Standard_EXPORT virtual Standard_Real ComputeValue() const;

  Standard_EXPORT virtual void Compute (const Handle(PrsMgr_PresentationManager3d)& thePresentationManager,
                                        const Handle(Prs3d_Presentation)& thePresentation,
                                        const Standard_Integer theMode = 0);

protected:

  Standard_EXPORT Standard_Boolean IsValidCircle (const gp_Circ& theCircle) const;

  Standard_EXPORT Standard_Boolean IsValidAnchor (const gp_Circ& theCircle,
                                                  const gp_Pnt& thePnt) const;

private:

  gp_Circ      myCircle;
  gp_Pnt       myAnchorPoint;
  TopoDS_Shape myShape;
};

#endif // _AIS_RadiusDimension_HeaderFile
