<?php
/*
Plugin Name: WanwanHouse Post Type v2
Description: Q&A post type (local/dev only)
Author: Simon - Azet
Version: v1.0
*/

// Q&A
function wp_menu_admin_wanwanhouse_qa(){
    $label = array(
        'name' => 'Q&A',
        'singular_name' => 'Q&A'
    );

    $args = array(
        'labels' => $label,
        'description' => 'Post Q&A Type',
        'supports' => array(
            'title',
            'editor',
            'excerpt',
            'author',
            'thumbnail',
            'comments',
            'trackbacks',
            'revisions',
            'custom-fields'
        ),
        'taxonomies' => array( 'loai_qa' ),
        'hierarchical' => false,
        'public' => true,
        'show_ui' => true,
        'show_in_menu' => true,
        'show_in_nav_menus' => false,
        'show_in_admin_bar' => false,
        'menu_position' => 5,
        'menu_icon' => 'dashicons-admin-page',
        'can_export' => true,
        'has_archive' => true,
        'exclude_from_search' => false,
        'publicly_queryable' => true,
        'capability_type' => 'post'
    );

    register_post_type('post_qa', $args);

}
add_action( 'init', 'wp_menu_admin_wanwanhouse_qa' );


function create_taxonomy_qa() {
    $taxonomylabels = array(
        'name' => _x('Q&Aカテゴリー','Q&Aカテゴリー'),
        'singular_name' => _x('Q&Aカテゴリー','Q&Aカテゴリー'),
        'search_items' => __('Q&Aカテゴリー探す'),
        'all_items' => __('全てのQ&Aカテゴリー'),
        'edit_item' => __('Q&Aカテゴリー更新'),
        'add_new_item' => __('カテゴリー追加'),
        'menu_name' => __('Q&Aカテゴリー'),
    );
    $args = array(
        'labels' => $taxonomylabels,
        'hierarchical' => true,
				'show_admin_column' => true,
    );
    register_taxonomy('loai_qa','qa',$args);
}
add_action ('init','create_taxonomy_qa',0);


// // Blog (import only then OFF)
// function wp_menu_admin_wanwanhouse_blog(){
// 	$label = array(
// 			'name' => 'Blog',
// 			'singular_name' => 'Blog'
// 	);

// 	$args = array(
// 			'labels' => $label,
// 			'description' => 'Post Blog Type',
// 			'supports' => array(
// 					'title',
// 					'editor',
// 					'excerpt',
// 					'author',
// 					'thumbnail',
// 					'comments',
// 					'trackbacks',
// 					'revisions',
// 					'custom-fields'
// 			),
// 			'taxonomies' => array( 'loai_blog' ),
// 			'hierarchical' => false,
// 			'public' => true,
// 			'show_ui' => true,
// 			'show_in_menu' => true,
// 			'show_in_nav_menus' => false,
// 			'show_in_admin_bar' => false,
// 			'menu_position' => 5,
// 			'menu_icon' => 'dashicons-admin-page',
// 			'can_export' => true,
// 			'has_archive' => true,
// 			'exclude_from_search' => false,
// 			'publicly_queryable' => true,
// 			'rewrite'   => array('slug' => 'blog'),
// 			'capability_type' => 'post'
// 	);
// 	register_post_type('post_blog', $args);
// }
// add_action( 'init', 'wp_menu_admin_wanwanhouse_blog' );


// trimming-menu
function wp_menu_admin_wanwanhouse_trimming(){
	$label = array(
			'name' => 'トリミングメニュー',
			'singular_name' => 'トリミングメニュー'
	);

	$args = array(
			'labels' => $label,
			'description' => 'Trimming details',
			'supports' => array(
					'title',
					'editor',
					'excerpt',
					'author',
					'thumbnail',
					'comments',
					'trackbacks',
					'revisions',
					'custom-fields'
			),
			'hierarchical' => false,
			'public' => true,
			'show_ui' => true,
			'show_in_menu' => true,
			'show_in_nav_menus' => false,
			'show_in_admin_bar' => false,
			'menu_position' => 5,
			'menu_icon' => 'dashicons-admin-page',
			'can_export' => true,
			'has_archive' => true,
			'exclude_from_search' => false,
			'publicly_queryable' => true,
			'rewrite'   => array('slug' => 'trimming-menu'),
			'capability_type' => 'post'
	);

	register_post_type('trimming_menu', $args);

}
add_action( 'init', 'wp_menu_admin_wanwanhouse_trimming' );
