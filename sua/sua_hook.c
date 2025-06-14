#include "sua.h"

void	sua_hook(t_xvar *xvar, int x_event, void (*funct)(), void *param)
{
	int	x_mask;

	if (x_event < 0 || x_event >= LASTEvent)
		return ;
	switch (x_event)
	{
		case KeyPress:
			x_mask = KeyPressMask;
			break ;
		case KeyRelease:
			x_mask = KeyReleaseMask;
			break ;
		case ButtonPress:
			x_mask = ButtonPressMask;
			break ;
		case ButtonRelease:
			x_mask = ButtonReleaseMask;
			break ;
		case MotionNotify:
			x_mask = PointerMotionMask;
			break ;
		case EnterNotify:
			x_mask = EnterWindowMask;
			break ;
		case LeaveNotify:
			x_mask = LeaveWindowMask;
			break ;
		case FocusIn:
		case FocusOut:
			x_mask = FocusChangeMask;
			break ;
		case KeymapNotify:
			x_mask = KeymapStateMask;
			break ;
		case Expose:
		case GraphicsExpose:
		case NoExpose:
			x_mask = ExposureMask;
			break ;
		case VisibilityNotify:
			x_mask = VisibilityChangeMask;
			break ;
		case CreateNotify:
		case DestroyNotify:
		case UnmapNotify:
		case MapNotify:
		case MapRequest:
		case ReparentNotify:
		case ConfigureNotify:
		case GravityNotify:
		case CirculateNotify:
		case CirculateRequest:
			x_mask = StructureNotifyMask;
			break ;
		case ConfigureRequest:
		case ResizeRequest:
			x_mask = ResizeRedirectMask;
			break ;
		case PropertyNotify:
			x_mask = PropertyChangeMask;
			break ;
		case SelectionClear:
		case SelectionRequest:
		case SelectionNotify:
			x_mask = OwnerGrabButtonMask;
			break ;
		case ColormapNotify:
			x_mask = ColormapChangeMask;
			break ;
		default:
			x_mask = NoEventMask;
	}
	xvar->hooks[x_event].hook = funct;
	xvar->hooks[x_event].param = param;
	xvar->hooks[x_event].mask = x_mask;
	return ;
}
