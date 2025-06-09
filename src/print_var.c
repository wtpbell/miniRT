/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print_var.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 09:34:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/06 16:48:59 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "debug/rt_debug.h"
#include "minirt.h"

void	mat4x4_rot_print(t_mat4x4 m)
{
	const t_v3f	right = init_v3f(1.0f, 0.0f, 0.0f);
	const t_v3f	up = init_v3f(0.0f, 1.0f, 0.0f);
	const t_v3f	forward = init_v3f(0.0f, 0.0f, 1.0f);
	t_v3f		angles;

	angles.x = acosf(v3f_dot(init_v3f(m[0], m[1], m[2]), right)) * RADTODEG;
	angles.y = acosf(v3f_dot(init_v3f(m[4], m[5], m[6]), up)) * RADTODEG;
	angles.z = acosf(v3f_dot(init_v3f(m[8], m[9], m[10]), forward)) * RADTODEG;
	v3f_print(angles, 0, "");
}

void	v3f_print(t_v3f v, int spaces, const char *prefix)
{
	printf("%*s%s(v3f) <%.4f, %.4f, %.4f>\n", spaces, "", prefix, v.x, v.y, v.z);
}

void	col32_print(t_v3f c, int spaces, const char *prefix)
{
	printf("%*s%s(color) <%.3f, %.3f, %.3f>\n", spaces, "", prefix, c.x, c.y, c.z);
}

void	float_print(float f, int spaces, const char *prefix)
{
	printf("%*s%s(flt) <%.2f>\n", spaces, "", prefix, f);
}

void	str_print(const char *str, int spaces, const char *prefix)
{
	printf("%*s%s %s\n", spaces, "", prefix, str);
}

void	mat4x4_print(t_mat4x4 m, int spaces, const char *prefix)
{
	printf("%*s%s\n", spaces, "", prefix);
	spaces += 2;
	printf("%*s|%f %f %f %f|\n", spaces, "", m[0], m[1], m[2], m[3]);
	printf("%*s|%f %f %f %f|\n", spaces, "", m[4], m[5], m[6], m[7]);
	printf("%*s|%f %f %f %f|\n", spaces, "", m[8], m[9], m[10], m[11]);
	printf("%*s|%f %f %f %f|\n", spaces, "", m[12], m[13], m[14], m[15]);
}

void	transform_print(t_trans	*t, int spaces)
{
	printf("%*s%s", spaces, "","TRANSFORM:\n");
	v3f_print(t->pos, spaces + 2, "POS");
	v3f_print(t->dir, spaces + 2, "DIR");
}

void	camera_print(t_cam *cam, int spaces)
{
	printf("%*s%s", spaces, "","CAMERA:\n");
	transform_print(&cam->t, spaces + 2);
	mat4x4_print(cam->view_matrix, spaces + 2, "view_matrix");
	float_print(cam->fov, spaces + 2, "FOV");
	col32_print(cam->bg_color, spaces + 2, "bg_color");
	
	// Print DoF parameters
	float_print(cam->aperture, spaces + 2, "APERTURE");
	float_print(cam->focus_dist, spaces + 2, "FOCUS_DIST");
	v3f_print(cam->u, spaces + 2, "U (right)");
	v3f_print(cam->v, spaces + 2, "V (up)");
	v3f_print(cam->w, spaces + 2, "W (forward)");
	v3f_print(cam->horizontal, spaces + 2, "HORIZONTAL");
	v3f_print(cam->vertical, spaces + 2, "VERTICAL");
	v3f_print(cam->lower_left, spaces + 2, "LOWER_LEFT");
}

void	plane_print(t_obj *pl, int spaces)
{
	printf("%*s%s", spaces, "","PLANE:\n");
	transform_print(&pl->t, spaces + 2);
}

void	sphere_print(t_obj *sp, int spaces)
{
	printf("%*s%s", spaces, "","SPHERE:\n");
	transform_print(&sp->t, spaces + 2);
	float_print(sp->sp.radius, spaces + 2, "RADIUS");
}

void	cylinder_print(t_obj *cy, int spaces)
{
	printf("%*s%s", spaces, "","CYLINDER:\n");
	transform_print(&cy->t, spaces + 2);
	float_print(cy->cy.radius, spaces + 2, "RADIUS");
	float_print(cy->cy.height, spaces + 2, "HEIGHT");
}

void	triangle_print(t_obj *tri, int spaces)
{
	printf("%*s%s", spaces, "","TRIANGLE:\n");
	transform_print(&tri->t, spaces + 2);
	v3f_print(tri->tri.v0, spaces + 2, "v0");
	v3f_print(tri->tri.v1, spaces + 2, "v1");
	v3f_print(tri->tri.v2, spaces + 2, "v2");
}

void	objects_print(t_vector	objects, int spaces, const char *prefix)
{
	int			i;
	t_obj	*obj;

	i = 0;
	printf("%*s%s <%i/%i>\n", spaces, "", prefix, objects.size, objects.capacity);
	while (i < objects.size)
	{
		obj = (t_obj *)objects.items[i];
		if (obj->type == OBJ_SPHERE)
			sphere_print(obj, spaces + 2);
		else if (obj->type == OBJ_PLANE)
			plane_print(obj, spaces + 2);
		else if (obj->type == OBJ_CYLINDER)
			cylinder_print(obj, spaces + 2);
		else if (obj->type == OBJ_TRIANGLE)
			triangle_print(obj, spaces + 2);
		col32_print(obj->r.color, spaces + 2, "COLOR");
		str_print(obj->r.mat->name, spaces + 2, "MATERIAL");
		
		++i;
	}
}

void	materials_print(t_vector materials, int spaces, const char *prefix)
{
	int		i;
	t_mat	*mat;

	i = 0;
	printf("%*s%s <%i/%i>\n", spaces, "", prefix, materials.size, materials.capacity);
	while (i < materials.size)
	{
		mat = (t_mat *)materials.items[i];
		str_print(mat->name, spaces + 2, "");
		if (mat->type == MAT_LAMBERTIAN)
		{
			str_print("Lambertian", spaces + 4, "type:");
			float_print(mat->lamb.specular, spaces + 4, "specular");
			float_print(mat->lamb.shininess, spaces + 4, "shininess");
		}
		else if (mat->type == MAT_METAL)
		{
			str_print("Metal", spaces + 4, "type:");
			float_print(mat->metal.fuzz, spaces + 4, "fuzz");
		}
		else if (mat->type == MAT_DIELECTRIC)
		{
			str_print("Dielectric", spaces + 4, "type:");
			float_print(mat->diel.ir, spaces + 4, "ir");
			float_print(mat->diel.transmittance, spaces + 4, "transmittance");
		}
		col32_print(mat->albedo, spaces + 4, "albedo");
		++i;
	}
}

void	scene_print(t_scene *scene)
{
	int	spaces = 0;

	printf("===SCENE===\n");
	camera_print(&scene->camera, spaces + 2);
	objects_print(scene->objects, spaces + 2, "OBJECTS");
	materials_print(scene->shared_materials, spaces + 2, "MATERIALS");
	printf("===END SCENE===\n");
}

void debug_scene_setup(t_scene *scene) {
	printf("\n=== SCENE SETUP DEBUG ===\n");
	printf("Camera position: (%.2f, %.2f, %.2f)\n", 
			scene->camera.t.pos.x, scene->camera.t.pos.y, scene->camera.t.pos.z);
	printf("Camera direction: (%.2f, %.2f, %.2f)\n", 
			scene->camera.t.dir.x, scene->camera.t.dir.y, scene->camera.t.dir.z);
	printf("FOV: %.2f\n", scene->camera.fov);
	printf("\nObjects (%d):\n", scene->objects.size);
	for (int i = 0; i < scene->objects.size; i++) {
		t_obj *obj = (t_obj *)scene->objects.items[i];
		printf("  [%d] Type: %d, Pos: (%.2f, %.2f, %.2f), Dir: (%.2f, %.2f, %.2f)\n",
				i, obj->type, 
				obj->t.pos.x, obj->t.pos.y, obj->t.pos.z,
				obj->t.dir.x, obj->t.dir.y, obj->t.dir.z);
		if (obj->r.mat) {
			printf("     Material: type=%d, albedo=(%.2f, %.2f, %.2f)\n",
					obj->r.mat->type,
					obj->r.mat->albedo.x,
					obj->r.mat->albedo.y,
					obj->r.mat->albedo.z);
		}
	}
	printf("\nLights (%d):\n", scene->lights.size);
	for (int i = 0; i < scene->lights.size; i++) {
		t_light *light = (t_light *)scene->lights.items[i];
		printf("  [%d] Type: %d, Pos: (%.2f, %.2f, %.2f), Color: (%.2f,%.2f,%.2f), Intensity: %.2f\n",
				i, light->type,
				light->pos.x, light->pos.y, light->pos.z,
				light->color.x, light->color.y, light->color.z,
				light->intensity);
	}
	printf("=========================\n\n");
}
