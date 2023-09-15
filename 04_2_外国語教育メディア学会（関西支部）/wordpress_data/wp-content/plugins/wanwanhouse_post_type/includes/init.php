<?php

//menu admin
require_once( PLUGIN_INC . '/admin.php');
// require_once( PLUGIN_INC . '/style.php');

class ActivePlugin
{
	/**
	 * Attached to activate_{ plugin_basename( __FILES__ ) } by register_activation_hook()
	 * @static
	 */
	public function plugin_activation() {
		return true;
	}

	/**
	 * Removes all connection options
	 * @static
	 */
	public function plugin_deactivation( ) {
		return true;
	}

}

$activeP = new ActivePlugin();
$activeP->plugin_activation();