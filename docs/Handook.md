# Overview
This will server as a living-document covering some of the design decisions made for the renderer.

# Lights
`Max Number Of Lights` was decided by researching the minimum number of lights needed in cinmatography in films. Where only the following 3 lights are needed, for a scene:
1. `Key Light` The primary light source used to illuminate the subject.
1. `Fill Light` This light is used to soften shadows created by the key light, providing a more natural and balanced look. It is positioned opposite the key light.
1. `Back Light` Placed behind the subject, the backlight adds depth by creating a separation between the subject and the background.

I've opted to add a 4th light, for the potential purpose of the sun/natural light. 