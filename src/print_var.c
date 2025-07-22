/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print_var.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 09:34:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/22 15:59:35 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "debug/rt_debug.h"
#include "material.h"
#include <stdio.h>
#include "parser.h"

#ifndef RT_DEBUG
# define RT_DEBUG 0  // Default to debug off if not defined
#endif
#include "minirt.h"
#include "MLX42/MLX42.h"

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

void	floats_print(float *f, int size, int spaces, const char *prefix)
{
	int	i;

	i = 0;
	printf("%*s%s(v%if) <", spaces, "", prefix, size);
	while (i < size)
	{
		printf("%.4f", f[i]);
		if (i < size - 1)
			printf(", ");
		++i;
	}
	printf(">\n");
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

void	cone_print(t_obj *co, int spaces)
{
	printf("%*s%s", spaces, "","CONE:\n");
	transform_print(&co->t, spaces + 2);
	float_print(co->cone.radius, spaces + 2, "RADIUS");
	float_print(co->cone.height, spaces + 2, "HEIGHT");
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
		else if (obj->type == OBJ_CONE)
			cone_print(obj, spaces + 2);
		col32_print(obj->r.color, spaces + 2, "COLOR");
		str_print(obj->r.mat->name, spaces + 2, "MATERIAL");

		++i;
	}
}

void	materials_print(t_vector materials, int spaces, const char *prefix)
{
	int			i;
	t_mat		*mat;
	t_tex		*tex;

	i = 0;
	printf("%*s%s <%i/%i>\n", spaces, "", prefix, materials.size, materials.capacity);
	while (i < materials.size)
	{
		mat = (t_mat *)materials.items[i];
		str_print(mat->name, spaces + 2, "");
		if (mat->type == MAT_LAMBERTIAN)
			str_print("Lambertian", spaces + 4, "type:");
		else if (mat->type == MAT_METAL)
		{
			str_print("Metal", spaces + 4, "type:");
			float_print(mat->metal.roughness, spaces + 4, "fuzz");
		}
		else if (mat->type == MAT_DIELECTRIC)
		{
			str_print("Dielectric", spaces + 4, "type:");
			float_print(mat->diel.ir, spaces + 4, "ir");
			float_print(mat->diel.transmittance, spaces + 4, "transmittance");
		}
		else
			str_print("Unknown", spaces + 4, "type:");

		// Print bump map information
		if (mat->bump_map)
		{
			str_print("Bump Map", spaces + 4, "");
			if (mat->bump_path)
				str_print(mat->bump_path, spaces + 6, "path:");
			printf("%*sdimensions: %dx%d\n", spaces + 6, "",
				mat->bump_map->width, mat->bump_map->height);
			printf("%*sscale: %.2f\n", spaces + 6, "", mat->bump_scale);
		}
		else if (mat->bump_path)
		{
			printf("%*sBump Map (failed to load): %s\n", spaces + 4, "", mat->bump_path);
		}
		col32_print(mat->albedo, spaces + 4, "albedo");

		tex = &mat->texture;
		if (tex->type == TEX_SOLID)
			str_print("SOLID", spaces + 4, "texture type:");
		else if (tex->type == TEX_CHECKER)
			str_print("CHECKER", spaces + 4, "texture type:");
		else if (tex->type == TEX_IMAGE)
			str_print("IMAGE", spaces + 4, "texture type:");
		else
			str_print("UNKNOWN", spaces + 4, "texture type:");
		col32_print(tex->col, spaces + 4, "alt col");
		floats_print(tex->scale_rot._axis, 3, spaces + 5, "uv scale rot");
		++i;
	}
}

void	print_camera_setup(t_cam *cam)
{
	printf("\n--- Camera Setup ---\n");
	printf("Position: (%.2f, %.2f, %.2f)\n",
		cam->t.pos.x, cam->t.pos.y, cam->t.pos.z);
	printf("Direction: (%.2f, %.2f, %.2f)\n",
		cam->t.dir.x, cam->t.dir.y, cam->t.dir.z);
	printf("Up: (%.2f, %.2f, %.2f)\n",
		cam->t.up.x, cam->t.up.y, cam->t.up.z);
	printf("U (right): (%.2f, %.2f, %.2f)\n",
		cam->u.x, cam->u.y, cam->u.z);
	printf("V (up): (%.2f, %.2f, %.2f)\n",
		cam->v.x, cam->v.y, cam->v.z);
	printf("W (forward): (%.2f, %.2f, %.2f)\n",
		cam->w.x, cam->w.y, cam->w.z);
	printf("Aperture: %.2f (from scene file)\n", cam->aperture);
	printf("Focus distance: %.2f (from scene file)\n", cam->focus_dist);
}

void	scene_print(t_scene *scene)
{
	if (!scene)
		return ;
	objects_print(scene->objects, 0, "OBJECTS");
	if (scene->shared_materials.size > 0)
		materials_print(scene->shared_materials, 0, "MATERIALS");
	camera_print(&scene->camera, 0);
	printf("=========================\n");
}

void	debug_bump_uv(const char *stage, t_v2f uv, float u_scale, float v_scale, float theta)
{
	if (!RT_DEBUG)
		return;
	printf("Bump %s: UV=(%.6f,%.6f)", stage, uv.u, uv.v);
	if (u_scale != 1.0f || v_scale != 1.0f || theta != 0.0f) {
		printf(", Scale=(%.2f,%.2f)", u_scale, v_scale);
		if (theta != 0.0f)
			printf(", Rot=%.1f°", theta);
	}
	printf("\n");
}

void	debug_bump_sample(mlx_texture_t *bump_map, t_v2f uv, int sample_idx)
{
	if (!RT_DEBUG || sample_idx >= 5)
		return;

	t_col32 tx = (t_col32)(uv.u * (bump_map->width - 1));
	t_col32 ty = (t_col32)(uv.v * (bump_map->height - 1));
	t_col32 idx = (ty * bump_map->width + tx) * bump_map->bytes_per_pixel;

	printf("Bump Sample #%d: UV(%.3f,%.3f) -> Texel(%d,%d) -> ",
		sample_idx + 1, uv.u, uv.v, tx, ty);

	if (bump_map->bytes_per_pixel >= 3) {
		printf("RGB(%d,%d,%d)",
			bump_map->pixels[idx],
			bump_map->pixels[idx + 1],
			bump_map->pixels[idx + 2]);
	} else {
		printf("Gray(%d)", bump_map->pixels[idx]);
	}
	printf("\n");
}

void	debug_bump_normal(t_v3f old_normal, t_v3f new_normal)
{
	printf("Bump normal: (%.3f,%.3f,%.3f) -> (%.3f,%.3f,%.3f)\n",
		old_normal.x, old_normal.y, old_normal.z,
		new_normal.x, new_normal.y, new_normal.z);
}

void	debug_bump_texture_info(mlx_texture_t *bump_map, float delta)
{
	printf("Bump map: Texture size: %dx%d, BPP: %d, Delta: %f\n",
		bump_map->width, bump_map->height, bump_map->bytes_per_pixel, delta);
}

void	debug_scene_setup(t_scene *scene) {
	// Scene setup debug information
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
