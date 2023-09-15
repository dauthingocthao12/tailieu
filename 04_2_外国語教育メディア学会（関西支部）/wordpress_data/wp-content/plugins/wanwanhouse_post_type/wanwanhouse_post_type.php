<?php
/*
Plugin Name: WanwanHouse Post Type
Description: Plugin created WanwanHouse Post Type, can custom post type.
Author: Hoang Do - Hachinet
Version: v1.0
*/

define( 'LD_VERSION', '1.0' );
define( 'LOCAL', $_SERVER['DOCUMENT_ROOT']);
define( 'LD_PLUGIN_DIR', plugin_dir_path( __FILE__ ) );
define( 'LD_PLUGIN_URL', plugins_url( plugin_basename(plugin_dir_path( __FILE__ )) ) );
define( 'PLUGIN_INC', LD_PLUGIN_DIR. '/includes' );
define( 'PLUGIN_STYLE', LD_PLUGIN_URL. '/assets' );
define( 'TEXT_DOMAIN', basename(get_template_directory()) );

require_once( PLUGIN_INC . '/init.php');


register_activation_hook( __FILE__, array( 'ActivePlugin', 'plugin_activation' ) );
register_deactivation_hook( __FILE__, array( 'ActivePlugin', 'plugin_deactivation' ) );


