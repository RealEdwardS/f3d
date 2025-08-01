#include "camera_impl.h"

#include <vtkCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkVersion.h>

namespace f3d::detail
{
class camera_impl::internals
{
public:
  //--------------------------------------------------------------------------
  // Orthogonalize view up even when the camera position->focal point vector
  // and up vector are collinear.
  static void OrthogonalizeViewUp(vtkCamera* cam)
  {
    vector3_t up;
    cam->GetViewUp(up.data());
    cam->OrthogonalizeViewUp();
    vector3_t orthogonalizedUp;
    cam->GetViewUp(orthogonalizedUp.data());
    static constexpr double EPSILON = 128 * std::numeric_limits<double>::epsilon();
    for (size_t i = 0; vtkMath::Norm(orthogonalizedUp.data()) < EPSILON && i < up.size(); i++)
    {
      // find closest up vector that cam->OrthogonalizeViewUp() does not transform into a zero
      // vector
      up[i] += EPSILON;
      if (i > 0)
      {
        // revert previous adjustment
        up[i - 1] -= EPSILON;
      }
      cam->SetViewUp(up.data());
      cam->OrthogonalizeViewUp();
      cam->GetViewUp(orthogonalizedUp.data());
    }
  }

  //--------------------------------------------------------------------------
  vtkRenderer* VTKRenderer = nullptr;
  camera_state_t DefaultCamera;
};

//----------------------------------------------------------------------------
camera_impl::camera_impl()
  : Internals(std::make_unique<camera_impl::internals>())
{
}

//----------------------------------------------------------------------------
camera_impl::~camera_impl() = default;

//----------------------------------------------------------------------------
camera& camera_impl::setPosition(const point3_t& pos)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->SetPosition(pos.data());
  this->Internals->OrthogonalizeViewUp(cam);
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
point3_t camera_impl::getPosition()
{
  point3_t pos;
  this->getPosition(pos);
  return pos;
}

//----------------------------------------------------------------------------
void camera_impl::getPosition(point3_t& pos)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->GetPosition(pos.data());
}

//----------------------------------------------------------------------------
camera& camera_impl::setFocalPoint(const point3_t& foc)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->SetFocalPoint(foc.data());
  this->Internals->OrthogonalizeViewUp(cam);
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
point3_t camera_impl::getFocalPoint()
{
  point3_t foc;
  this->getFocalPoint(foc);
  return foc;
}

//----------------------------------------------------------------------------
void camera_impl::getFocalPoint(point3_t& foc)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->GetFocalPoint(foc.data());
}

//----------------------------------------------------------------------------
camera& camera_impl::setViewUp(const vector3_t& up)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->SetViewUp(up.data());
  this->Internals->OrthogonalizeViewUp(cam);
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
vector3_t camera_impl::getViewUp()
{
  vector3_t up;
  this->getViewUp(up);
  return up;
}

//----------------------------------------------------------------------------
void camera_impl::getViewUp(vector3_t& up)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->GetViewUp(up.data());
}

//----------------------------------------------------------------------------
camera& camera_impl::setViewAngle(const angle_deg_t& angle)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->SetViewAngle(angle);
  this->Internals->OrthogonalizeViewUp(cam);
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
angle_deg_t camera_impl::getViewAngle()
{
  angle_deg_t angle;
  this->getViewAngle(angle);
  return angle;
}

//----------------------------------------------------------------------------
void camera_impl::getViewAngle(angle_deg_t& angle)
{
  vtkCamera* cam = this->GetVTKCamera();
  angle = cam->GetViewAngle();
}

//----------------------------------------------------------------------------
camera& camera_impl::setState(const camera_state_t& state)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->SetPosition(state.position.data());
  cam->SetFocalPoint(state.focalPoint.data());
  cam->SetViewUp(state.viewUp.data());
  cam->SetViewAngle(state.viewAngle);
  this->Internals->OrthogonalizeViewUp(cam);
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
camera_state_t camera_impl::getState()
{
  camera_state_t state;
  this->getState(state);
  return state;
}

//----------------------------------------------------------------------------
void camera_impl::getState(camera_state_t& state)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->GetPosition(state.position.data());
  cam->GetFocalPoint(state.focalPoint.data());
  cam->GetViewUp(state.viewUp.data());
  state.viewAngle = cam->GetViewAngle();
}
//----------------------------------------------------------------------------
camera& camera_impl::dolly(double val)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->Dolly(val);
  this->Internals->OrthogonalizeViewUp(cam);
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
camera& camera_impl::pan(double right, double up, double forward)
{
  vtkCamera* cam = this->GetVTKCamera();
  double pos[3], foc[3], vUp[3], vForward[3], vRight[3];
  cam->GetPosition(pos);
  cam->GetFocalPoint(foc);
  cam->GetViewUp(vUp);
  vtkMath::Subtract(foc, pos, vForward);
  vtkMath::Normalize(vForward);
  vtkMath::Cross(vForward, vUp, vRight);

  vtkMath::MultiplyScalar(vRight, right);
  vtkMath::MultiplyScalar(vUp, up);
  vtkMath::MultiplyScalar(vForward, forward);

  vtkMath::Add(pos, vRight, pos);
  vtkMath::Add(pos, vUp, pos);
  vtkMath::Add(pos, vForward, pos);
  cam->SetPosition(pos);

  vtkMath::Add(foc, vRight, foc);
  vtkMath::Add(foc, vUp, foc);
  vtkMath::Add(foc, vForward, foc);
  cam->SetFocalPoint(foc);

  this->Internals->OrthogonalizeViewUp(cam);
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
camera& camera_impl::zoom(double factor)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->Zoom(factor);
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
camera& camera_impl::roll(angle_deg_t angle)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->Roll(angle);
  this->Internals->OrthogonalizeViewUp(cam);
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
camera& camera_impl::azimuth(angle_deg_t angle)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->Azimuth(angle);
  this->Internals->OrthogonalizeViewUp(cam);
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
camera& camera_impl::yaw(angle_deg_t angle)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->Yaw(angle);
  this->Internals->OrthogonalizeViewUp(cam);
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
camera& camera_impl::elevation(angle_deg_t angle)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->Elevation(angle);
  this->Internals->OrthogonalizeViewUp(cam);
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
camera& camera_impl::pitch(angle_deg_t angle)
{
  vtkCamera* cam = this->GetVTKCamera();
  cam->Pitch(angle);
  this->Internals->OrthogonalizeViewUp(cam);
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
camera& camera_impl::setCurrentAsDefault()
{
  this->getState(this->Internals->DefaultCamera);
  return *this;
}

//----------------------------------------------------------------------------
camera& camera_impl::resetToDefault()
{
  this->setState(this->Internals->DefaultCamera);
  return *this;
}

//----------------------------------------------------------------------------
camera& camera_impl::resetToBounds([[maybe_unused]] double zoomFactor)
{
#if __ANDROID__
  this->Internals->VTKRenderer->ResetCamera();
#else
#if VTK_VERSION_NUMBER >= VTK_VERSION_CHECK(9, 2, 20230221)
  this->Internals->VTKRenderer->ResetCameraScreenSpace(zoomFactor);
#else
  this->Internals->VTKRenderer->ResetCameraScreenSpace();
#endif
#endif
  this->Internals->VTKRenderer->ResetCameraClippingRange();
  return *this;
}

//----------------------------------------------------------------------------
void camera_impl::SetVTKRenderer(vtkRenderer* renderer)
{
  this->Internals->VTKRenderer = renderer;
}

//----------------------------------------------------------------------------
vtkCamera* camera_impl::GetVTKCamera()
{
  return this->Internals->VTKRenderer->GetActiveCamera();
}
};
