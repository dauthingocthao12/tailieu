<?php
// === リソース ===
	
// CSS (common)
wp_enqueue_style( 'font-awesome', get_template_directory_uri().'/fontawesome-free-5.15.3-web/css/all.min.css');
// adminでcssがぶつかるのを避けるために入れる
if(is_admin()===false) {
	// CSS
	wp_enqueue_style( 'mobile-menu-css', get_template_directory_uri().'/mobile_menu/mobile_menu.css');
	wp_enqueue_style( 'style', get_stylesheet_uri().'' );

	// JS
	wp_enqueue_script('top-arrow', get_template_directory_uri().'/js/top-arrow.js', ['jquery']);
	wp_enqueue_script('fixed-menu', get_template_directory_uri().'/js/fixed-menu.js', ['jquery']);
	wp_enqueue_script('mobile-menu-js', get_template_directory_uri().'/mobile_menu/mobile_menu.js', ['jquery']);
	wp_enqueue_script('main', get_template_directory_uri().'/js/main.js', ['jquery', 'top-arrow','fixed-menu', 'mobile-menu-js']);
}

// === WP設定 / レイアウト ===

// サムネイル
add_theme_support( 'post-thumbnails' );

// メニュー
register_nav_menus([
	'side_submenu' => 'サイドサブメニュー',
	'side_links' => 'サイドリンク',
	'footer' => 'フッターメニュー',
]);

/**
* バージョンアップ通知の非表示
*/
function update_nag_hide() {
remove_action( 'admin_notices', 'update_nag', 3 );
remove_action( 'admin_notices', 'maintenance_nag', 10 );
}
add_action( 'admin_init', 'update_nag_hide' );

define("BLOG_SLUG", "blog"); // お知らせ・ブログなど









