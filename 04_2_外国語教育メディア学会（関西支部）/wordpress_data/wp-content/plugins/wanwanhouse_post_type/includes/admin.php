<?php

// Voice list
function wp_menu_admin_wanwanhouse_voice_list(){
    $label = array(
        'name' => 'お客様の声',
        'singular_name' => 'お客様の声'
    );

    $args = array(
        'labels' => $label,
        'description' => 'Post Voice Type',
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
        'taxonomies' => array( 'loai_voice' ),
        'hierarchical' => false,
        'public' => true, 
        'show_ui' => true,
        'show_in_menu' => true, 
        'show_in_nav_menus' => false, 
        'show_in_admin_bar' => false,
        'menu_position' => 5, 
        'menu_icon' => 'dashicons-admin-page', 
        'can_export' => false, 
        'has_archive' => true,
        'exclude_from_search' => false,
        'publicly_queryable' => true, 
        'capability_type' => 'post'
    );
 
    register_post_type('post_voice', $args);

}
// add_action( 'init', 'wp_menu_admin_wanwanhouse_voice_list' );

//function create_taxonomy_voice() {
//    $taxonomylabels = array( 
//        'name' => _x('お客様の声カテゴリー','お客様の声カテゴリー'),
//        'singular_name' => _x('お客様の声カテゴリー','お客様の声カテゴリー'),
//        'search_items' => __('お客様の声カテゴリー探す'),
//        'all_items' => __('全ての お客様の声カテゴリー'),
//        'edit_item' => __('お客様の声カテゴリー更新'),
//        'add_new_item' => __('カテゴリー追加'),
//        'menu_name' => __('お客様の声カテゴリー'),
//    );
//    $args = array(
//        'labels' => $taxonomylabels,
//        'hierarchical' => true,
//    );
//    register_taxonomy('loai_voice','voice',$args);
//}
//add_action ('init','create_taxonomy_voice',0);

// Hotel
function wp_menu_admin_wanwanhouse_hotel(){
    $label = array(
        'name' => 'ドッグホテルお部屋一覧',
        'singular_name' => 'ドッグホテルお部屋一覧' 
    );

    $args = array(
        'labels' => $label,
        'description' => 'Post Hotel Type', 
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
        'taxonomies' => array( 'loai_hotel' ), 
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
 
    register_post_type('post_hotel', $args);

}
// add_action( 'init', 'wp_menu_admin_wanwanhouse_hotel' );

// function create_taxonomy_hotel() {
//     $taxonomylabels = array( 
//         'name' => _x('Hotel Type','Hotel Type'),
//         'singular_name' => _x('Hotel Type','Hotel Type'),
//         'search_items' => __('Find Hotel Type'),
//         'all_items' => __('All Hotels Type'),
//         'edit_item' => __('Edit Hotel Type'),
//         'add_new_item' => __('カテゴリー追加'),
//         'menu_name' => __('Hotel Type'),
//     );
//     $args = array(
//         'labels' => $taxonomylabels,
//         'hierarchical' => true,
//     );
//     register_taxonomy('loai_hotel','hotel',$args);
// }
// add_action ('init','create_taxonomy_hotel',0);

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
    );
    register_taxonomy('loai_qa','qa',$args);
}
add_action ('init','create_taxonomy_qa',0);

// Trimming
// function wp_menu_admin_wanwanhouse_trimming(){
//     $label = array(
//         'name' => 'Trimming',
//         'singular_name' => 'Trimming'
//     );

//     $args = array(
//         'labels' => $label,
//         'description' => 'Post Trimming type',
//         'supports' => array(
//             'title',
//             'editor',
//             'excerpt',
//             'author',
//             'thumbnail',
//             'comments',
//             'trackbacks',
//             'revisions',
//             'custom-fields'
//         ),
//         'taxonomies' => array( 'loai_trimming' ),
//         'hierarchical' => false,
//         'public' => true,
//         'show_ui' => true,
//         'show_in_menu' => true,
//         'show_in_nav_menus' => false,
//         'show_in_admin_bar' => false,
//         'menu_position' => 5,
//         'menu_icon' => 'dashicons-admin-page',
//         'can_export' => true,
//         'has_archive' => true,
//         'exclude_from_search' => false,
//         'publicly_queryable' => true,
//         'capability_type' => 'post'
//     );

//     register_post_type('post_trimming', $args);

// }
// add_action( 'init', 'wp_menu_admin_wanwanhouse_trimming' );

// function create_taxonomy_trimming() {
//     $taxonomylabels = array(
//         'name' => _x('Loại Trimming','Loại Trimming'),
//         'singular_name' => _x('Loại Trimming','Loại Trimming'),
//         'search_items' => __('Tìm Loại Trimming'),
//         'all_items' => __('Tất cả Loại Trimming'),
//         'edit_item' => __('Sửa Loại Trimming'),
//         'add_new_item' => __('Thêm loại mới'),
//         'menu_name' => __('Loại Trimming'),
//     );
//     $args = array(
//         'labels' => $taxonomylabels,
//         'hierarchical' => true,
//     );
//     register_taxonomy('loai_trimming','trimming',$args);
// }
// add_action ('init','create_taxonomy_trimming',0);

// Shampoo Car
// function wp_menu_admin_wanwanhouse_shampoo_car(){
//     $label = array(
//         'name' => 'Shampoo Car',
//         'singular_name' => 'Shampoo Car'
//     );

//     $args = array(
//         'labels' => $label,
//         'description' => 'Post type đăng Shampoo Car',
//         'supports' => array(
//             'title',
//             'editor',
//             'excerpt',
//             'author',
//             'thumbnail',
//             'comments',
//             'trackbacks',
//             'revisions',
//             'custom-fields'
//         ),
//         'taxonomies' => array( 'loai_shampoo_car' ),
//         'hierarchical' => false,
//         'public' => true,
//         'show_ui' => true,
//         'show_in_menu' => true,
//         'show_in_nav_menus' => false,
//         'show_in_admin_bar' => false,
//         'menu_position' => 5,
//         'menu_icon' => 'dashicons-admin-page',
//         'can_export' => true,
//         'has_archive' => true,
//         'exclude_from_search' => false,
//         'publicly_queryable' => true,
//         'capability_type' => 'post'
//     );

//     register_post_type('post_shampoo_car', $args);

// }
// add_action( 'init', 'wp_menu_admin_wanwanhouse_shampoo_car' );

// function create_taxonomy_shampoo_car() {
//     $taxonomylabels = array(
//         'name' => _x('Loại Shampoo Car','Loại Shampoo Car'),
//         'singular_name' => _x('Loại Shampoo Car','Loại Shampoo Car'),
//         'search_items' => __('Tìm Loại Shampoo Car'),
//         'all_items' => __('Tất cả Loại Shampoo Car'),
//         'edit_item' => __('Sửa Loại Shampoo Car'),
//         'add_new_item' => __('Thêm loại mới'),
//         'menu_name' => __('Loại Shampoo Car'),
//     );
//     $args = array(
//         'labels' => $taxonomylabels,
//         'hierarchical' => true,
//     );
//     register_taxonomy('loai_shampoo_car','shampoo_car',$args);
// }
// add_action ('init','create_taxonomy_shampoo_car',0);

// About
// function wp_menu_admin_wanwanhouse_about(){
//     $label = array(
//         'name' => 'About',
//         'singular_name' => 'About'
//     );

//     $args = array(
//         'labels' => $label,
//         'description' => 'Post type đăng About',
//         'supports' => array(
//             'title',
//             'editor',
//             'excerpt',
//             'author',
//             'thumbnail',
//             'comments',
//             'trackbacks',
//             'revisions',
//             'custom-fields'
//         ),
//         'taxonomies' => array( 'loai_about' ),
//         'hierarchical' => false,
//         'public' => true,
//         'show_ui' => true,
//         'show_in_menu' => true,
//         'show_in_nav_menus' => false,
//         'show_in_admin_bar' => false,
//         'menu_position' => 5,
//         'menu_icon' => 'dashicons-admin-page',
//         'can_export' => true,
//         'has_archive' => true,
//         'exclude_from_search' => false,
//         'publicly_queryable' => true,
//         'capability_type' => 'post'
//     );

//     register_post_type('post_about', $args);

// }
// add_action( 'init', 'wp_menu_admin_wanwanhouse_about' );

// function create_taxonomy_about() {
//     $taxonomylabels = array(
//         'name' => _x('Loại About','Loại About'),
//         'singular_name' => _x('Loại About','Loại About'),
//         'search_items' => __('Tìm Loại About'),
//         'all_items' => __('Tất cả Loại About'),
//         'edit_item' => __('Sửa Loại About'),
//         'add_new_item' => __('Thêm loại mới'),
//         'menu_name' => __('Loại About'),
//     );
//     $args = array(
//         'labels' => $taxonomylabels,
//         'hierarchical' => true,
//     );
//     register_taxonomy('loai_about','about',$args);
// }
// add_action ('init','create_taxonomy_about',0);

// Contact
// function wp_menu_admin_wanwanhouse_contact(){
//     $label = array(
//         'name' => 'Contact',
//         'singular_name' => 'Contact'
//     );

//     $args = array(
//         'labels' => $label,
//         'description' => 'Post type đăng Contact',
//         'supports' => array(
//             'title',
//             'editor',
//             'excerpt',
//             'author',
//             'thumbnail',
//             'comments',
//             'trackbacks',
//             'revisions',
//             'custom-fields'
//         ),
//         'taxonomies' => array( 'loai_contact' ),
//         'hierarchical' => false,
//         'public' => true,
//         'show_ui' => true,
//         'show_in_menu' => true,
//         'show_in_nav_menus' => false,
//         'show_in_admin_bar' => false,
//         'menu_position' => 5,
//         'menu_icon' => 'dashicons-admin-page',
//         'can_export' => true,
//         'has_archive' => true,
//         'exclude_from_search' => false,
//         'publicly_queryable' => true,
//         'capability_type' => 'post'
//     );

//     register_post_type('post_contact', $args);

// }
// add_action( 'init', 'wp_menu_admin_wanwanhouse_contact' );

// function create_taxonomy_contact() {
//     $taxonomylabels = array(
//         'name' => _x('Loại Contact','Loại Contact'),
//         'singular_name' => _x('Loại Contact','Loại Contact'),
//         'search_items' => __('Tìm Loại Contact'),
//         'all_items' => __('Tất cả Loại Contact'),
//         'edit_item' => __('Sửa Loại Contact'),
//         'add_new_item' => __('Thêm loại mới'),
//         'menu_name' => __('Loại Contact'),
//     );
//     $args = array(
//         'labels' => $taxonomylabels,
//         'hierarchical' => true,
//     );
//     register_taxonomy('loai_contact','contact',$args);
// }
// add_action ('init','create_taxonomy_contact',0);

// // Blog
// function wp_menu_admin_wanwanhouse_blog(){
//     $label = array(
//         'name' => 'Blog',
//         'singular_name' => 'Blog'
//     );

//     $args = array(
//         'labels' => $label,
//         'description' => 'Post Blog Type',
//         'supports' => array(
//             'title',
//             'editor',
//             'excerpt',
//             'author',
//             'thumbnail',
//             'comments',
//             'trackbacks',
//             'revisions',
//             'custom-fields'
//         ),
//         'taxonomies' => array( 'loai_blog' ),
//         'hierarchical' => false,
//         'public' => true,
//         'show_ui' => true,
//         'show_in_menu' => true,
//         'show_in_nav_menus' => false,
//         'show_in_admin_bar' => false,
//         'menu_position' => 5,
//         'menu_icon' => 'dashicons-admin-page',
//         'can_export' => true,
//         'has_archive' => true,
//         'exclude_from_search' => false,
//         'publicly_queryable' => true,
//         'rewrite'   => array('slug' => 'blog'),
//         'capability_type' => 'post'
//     );

//     register_post_type('post_blog', $args);

// }
// add_action( 'init', 'wp_menu_admin_wanwanhouse_blog' );

// function create_taxonomy_blog() {
//    $taxonomylabels = array(
//     'name' => _x('Blogカテゴリー','Blogカテゴリー'),
//     'singular_name' => _x('Blogカテゴリー','Blogカテゴリー'),
//     'search_items' => __('Blog カテゴリー探す'),
//     'all_items' => __('全てのBlogカテゴリー'),
//     'edit_item' => __('Blogカテゴリー更新'),
//     'add_new_item' => __('カテゴリー追加'),
//     'menu_name' => __('Blogカテゴリー'),
//    );
//    $args = array(
//        'labels' => $taxonomylabels,
//        'hierarchical' => true,
//    );
//    register_taxonomy('loai_blog','blog',$args);
// }
// add_action ('init','create_taxonomy_blog',0);


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