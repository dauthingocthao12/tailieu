<?php 

use TheIconic\Tracking\GoogleAnalytics\Analytics;
require_once __DIR__ . '/vendor/autoload.php';
require_once dirname(__FILE__)."/../../uranai_lib/libs/Smarty.class.php" ;


function user_tracking($referer){

    $analytics = new Analytics();
    $id_dev = 'UA-573797-15';
    $id_prod = 'UA-573797-12';
//     $clientID = $_SERVER['REMOTE_ADDR'];
    $time = strtotime("now");
    if(IS_SERVER)  {  
        $id = $id_prod;
        $home_page = BASE_URL;
    }else  {
        $id = $id_dev;
        $home_page = DEV_SITE_ROOT_URL;
    }

    $analytics->setProtocolVersion('1')
            ->setTrackingId($id)
            ->setClientId($time);

    $analytics->setEventCategory('user_tracking')
            ->setEventAction('user_url')
            ->setEventLabel($referer)
            ->sendEvent();

    header('Location: ' . $home_page ); // ホームページに行く
    

    exit();
}

    
?>