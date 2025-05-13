**discretization:** breaking down a continuous object into segments (our case pixels)

**raster:** a xy grid; raster graphics, raster displays

**color quantization:** Matching the color with the closest color from the color palette that is available.

**banding:** continues gradients appear as a series of descrite steps or bands of color

**coplanar:** All points composing a face or polygon should lie on the same plane

**triangulation:** The process of converting faces into triangles

**sampling:** placing points or vertices along a smooth surface

**tessallation:** The process of converting a smooth surface to a triangle mesh

**ray tracing:** Ray tracing calculates the intersection of rays with scene geometry (geometric solution, algebraic solution)

**algebraic solution:** to the ray-object intersection test. This is feasible when the object's shape can be mathematically defined with an equation, typically representing a surface (implicit surfaces, e.g., a sphere's surface) across space.

>Materials are broadly categorized into two types: **conductors**, which are metals, and **dielectrics**, encompassing non-metals such as glass, plastic, wood, and water.

**forward ray tracing (light tracing):** tracing the path of photons from their source to the observer.

**backward ray-tracing** or **eye tracing**

**primary ray**, **visibility ray**, or **camera ray**

**secondary ray:** shadow ray, reflection ray, refraction ray

**shadow ray**: The ray from the point of contact onto the surface to the light source

>The technique of initiating rays either from the light source or from the eye is encapsulated by the term **path tracing** in computer graphics. While ray-tracing is a synonymous term, path tracing emphasizes the methodological essence of generating computer-generated imagery by tracing the journey of light from its source to the camera, or vice versa.

**reflection** and **refraction**

>Additionally, calculating the direction of refraction requires knowledge of the material's **index of refraction**. Refraction can be visualized as the bending of the ray's path when it transitions between mediums of differing refractive indices

>Here, the **Fresnel equation** plays a critical role, providing the formula needed to ascertain the appropriate mix of reflection and refraction.

>This three-step process—calculating reflection, calculating refraction, and applying the Fresnel equation—enables the realistic rendering of complex optical phenomena.

>... need a system that defines how these eight points are connected to make up the faces of the box. In CG, this is called the **topology** of the object (also known as a **model**)

>**Topology** refers to how points, which we call vertices, are connected to form faces (or flat surfaces), also known as polygons

>an imaginary canvas, a process known as **perspective projection**

**z or perspective divide**

advanced light-transport algorithms: the **path-tracing**, the **bidirectional path-tracing** and **Metropolis light transport**
