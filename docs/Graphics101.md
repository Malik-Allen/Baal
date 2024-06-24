# Intro
Realistic rendering simulates light using models from physics and computer science to create images.

## Images and Light
Energy emitted as light travels through space as a streams of particles(called a Photon) until is hits a surface. Then, the energy is either scattered or absored. 
The scattered energy travels through space until it hits another surface. Where it is again either scattered or absorbed. Again, and again, and again...

That scattered light eventually makes its way into our eyes. Where again, some light is absorbed and scattered. Our brain forms images based on the trillions of light particles that reach our eyes.

The same journey takes place when capturing an image using a camera.

## Physics-Based Rendering
![Ray Trace Car Demo](resources/RayTraceReflectionsCar.jpg)

The goal of rendering is to compute the energy transfered to each pixel on an image sensor in the virtual world.

Computing or processing trillions of particles in a simulation requires hardware that does not exist in today's world. So, for efficiency, we have created a model where we combine actions of many photons at the same time.

We create an equation, that given some light sources, considers the energy that arrives at a camera sensor. It accounts for direction of the light, the area of the light, the size of the pixel, etc. This equation is called the `Rendering Equation`.
Through this model, we don't have to consider the trillions of particles of light in a given scene.

## The Rendering Equation
![The Rendering Equation](resources/TheRenderingEquationDiagram.png)

Consider a simple scene, as shown in the above diagram. A single light source, two surfaces, a virtual image plane, and camera.

When light emitted from the light source hits either surface, some light is scattered or absorbed. There are many paths for light to make its way into the camera. Some light is scattered across multiple surfaces, some light is emitted directly into the camera, etc.

Consider one of the possible paths, where a light ray is bounced from surface Y onto surface X and then into the camera.
Where that light ray is measured by the camera sensor. 

The measured light ray from the last surface along a path is called the `Primary Ray`.

This is the Rendering Equation. All complicated paths towards cameras/eyes eventually become a Primary Ray and we are trying to solve that equation as efficiently as possible.

## Light Radiation
Light is `electromagnetic radiation`. Light can propagate along a ray through a medium. For example, we can see through glass, air, and water because light propagrates through them.

![Electromagnetic Spectrum](resources/ElectromagneticSpectrum.png)

How `transmissive` an object is varies with the frequency of light. For example, we are not able to see through our skin and muscle tissues, because visible light radiation is unable to propagate through them.
However, we can see through our skin and muscle tissues using x-ray radiation. The frequency of x-ray radiation allows it to propagate through our skin. We we can then convert the x-ray radiation into visible light radiation.

Not all objects are uniform in their transmissivness. For example, there are some areas of our skin tissue that some light is able to propagate through. Like your ears, nose, parts of your hand, etc. This is called `subsurface scattering`.

![Subsurface Scattering](resources/SubsurfaceScattering.jpg)

The speed of light will change depending on the medium it propagrates through. For example, the speed of light is 1.3 times slower in water and 2.5 times slower when propagating through a diamond.

## Surface Geometry
We can model an `object` as a collection of surfaces in computer grpahics. We are not concerned about any of the innards, mass, etc of the object. Our only concern is what is visible.

We can represent a surface through the following models:

1. `Geometry` models the space occupied by a surface.
2. `Materials` models its interactions with light.

### What is a surface?
An `surface` is a 2D boundary between adjascent volumes of two different, homogenous media. A generalization of the mathematical plane, however a surface can be curved.

A surface is be modeled as `single-sided`. In that way a surface will have a direction that it faces. Again, identical to a plane, where the normal to the plane defines the direction its facing.

### Triangle Meshes
In 3-Dimensions, a triangle is the simplest polygon (known as `simplex`). Triangles serve as the building blocks for surfaces, as any higher-order polygon can be broken down into triangles.

A single triangle consists of 3 points. A surface can be built using of a set of triangles, also known as a `mesh`.

The following is a Tetrahedron, consisiting of 3 triangles:

![Tetrahedron](resources/Tetrahedron.png)

Remember, a surface is single-sided. By convention, the `front` of a triangle is defined as the side from which its vertices are seen be ordered `counter-clockwise` (the right-hand rule).

![Indexed Triangle Mesh](resources/IndexedTriangleMesh.png)

We use `mesh indecies` to maintain the order of vertices of the triangles of mesh.

## Resources

https://graphicscodex.com/app/app.html?page=_rn_preface

https://en.wikipedia.org/wiki/Transmittance