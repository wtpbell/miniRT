/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 09:34:02 by jboon             #+#    #+#             */
/*   Updated: 2025/05/31 23:34:59 by bewong           ###   ########.fr       */
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

void	col32_print(t_col32 c, int spaces, const char *prefix)
{
	printf("%*s%s(col32) <%i, %i, %i, %i>\n", spaces, "", prefix, get_r(c), get_g(c), get_b(c), get_a(c));
}

void	float_print(float f, int spaces, const char *prefix)
{
	printf("%*s%s(flt) <%.2f>\n", spaces, "", prefix, f);
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
	col32_print(cam->bg_col, spaces + 2, "bg_col");
}

void	plane_print(t_obj *pl, int spaces)
{
	printf("%*s%s", spaces, "","PLANE:\n");
	transform_print(&pl->t, spaces + 2);
	col32_print(pl->r.col, spaces + 2, "COL");
}

void	sphere_print(t_obj *sp, int spaces)
{
	printf("%*s%s", spaces, "","SPHERE:\n");
	transform_print(&sp->t, spaces + 2);
	float_print(sp->sp.radius, spaces + 2, "RADIUS");
	col32_print(sp->r.col, spaces + 2, "COL");
}

void	cylinder_print(t_obj *cy, int spaces)
{
	printf("%*s%s", spaces, "","CYLINDER:\n");
	transform_print(&cy->t, spaces + 2);
	float_print(cy->cy.radius, spaces + 2, "RADIUS");
	float_print(cy->cy.height, spaces + 2, "HEIGHT");
	col32_print(cy->r.col, spaces + 2, "COL");
}

void	triangle_print(t_obj *tri, int spaces)
{
	printf("%*s%s", spaces, "","TRIANGLE:\n");
	transform_print(&tri->t, spaces + 2);
	v3f_print(tri->tri.v0, spaces + 2, "v0");
	v3f_print(tri->tri.v1, spaces + 2, "v1");
	v3f_print(tri->tri.v2, spaces + 2, "v2");
	col32_print(tri->r.col, spaces + 2, "COL");
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
		++i;
	}
}

void	scene_print(t_scene *scene)
{
	int	spaces = 0;

	printf("===SCENE===\n");
	camera_print(&scene->camera, spaces + 2);
	objects_print(scene->objects, spaces + 2, "OBJECTS");
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
		printf("  [%d] Type: %d, Pos: (%.2f, %.2f, %.2f), Color: (%d,%d,%d), Intensity: %.2f\n",
				i, light->type,
				light->pos.x, light->pos.y, light->pos.z,
				get_r(light->col), get_g(light->col), get_b(light->col),
				light->intensity);
	}
	printf("=========================\n\n");
}
