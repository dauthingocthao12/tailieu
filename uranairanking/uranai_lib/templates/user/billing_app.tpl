<!DOCTYPE html>
<!--[if lt IE 7]>      <html class="no-js lt-ie9 lt-ie8 lt-ie7"> <![endif]-->
<!--[if IE 7]>         <html class="no-js lt-ie9 lt-ie8"> <![endif]-->
<!--[if IE 8]>         <html class="no-js lt-ie9"> <![endif]-->
<!--[if gt IE 8]><!-->
<html class="no-js"> <!--<![endif]-->
<head>
    <!-- Google Tag Manager -->
    <script>(function(w,d,s,l,i){w[l]=w[l]||[];w[l].push({'gtm.start':
    new Date().getTime(),event:'gtm.js'});var f=d.getElementsByTagName(s)[0],
    j=d.createElement(s),dl=l!='dataLayer'?'&l='+l:'';j.async=true;j.src=
    'https://www.googletagmanager.com/gtm.js?id='+i+dl;f.parentNode.insertBefore(j,f);
    })(window,document,'script','dataLayer','GTM-NWDDK6C');</script>
    <!-- End Google Tag Manager -->
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    {block name="seo"}
    <title>12星座占いランキング</title>
    <meta name="keywords" content="12星座占いランキング,占い,ランキング">
    <meta name="description" content="各サイトから情報を取得して12星座占いのランキングを表示しております。">
    {/block}
	<meta property="og:image" content="http://uranairanking.jp/user/img_re/new-fb-thumbnail.jpg" />
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- Place favicon.ico and apple-touch-icon.png in the root directory -->
    <link rel="stylesheet" href="/user/css/normalize.css">
    <link rel="stylesheet" href="/user/css/main.css">
    <link rel="stylesheet" href="/user/packages/bootstrap-3.1.1-dist/css/bootstrap.min.css">
    <link rel="stylesheet" href="/user/packages/font-awesome-4.5.0/css/font-awesome.min.css">
    <!--app things-->
    {if $design_name.class_name == "designA"}
    <link rel="stylesheet" href="/user/css/app-main.css?var={$config.cache_date}">
    {else}
    <link rel="stylesheet" href="/user/css/{if $design_name}{$design_name.ev_name}{/if}app-main.css?var={$config.cache_date}">
	{/if}
	 <!-- style override for nav.navbar etc. -->
	{literal}
	<!-- ユーザーヒート -->
	<script type="text/javascript">
	(function(add, cla){window['UserHeatTag']=cla;window[cla]=window[cla]||function(){(window[cla].q=window[cla].q||[]).push(arguments)},window[cla].l=1*new Date();var ul=document.createElement('script');var tag = document.getElementsByTagName('script')[0];ul.async=1;ul.src=add;tag.parentNode.insertBefore(ul,tag);})('//uh.nakanohito.jp/uhj2/uh.js', '_uhtracker');_uhtracker({id:'uhlHJgq9kn'});
	</script>
	<!-- ユーザーヒート終了 -->
	{/literal}
	
	<script src="/user/js/vendor/jquery-1.12.0.min.js"></script>
	<script src="//ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
	<script src="//cdnjs.cloudflare.com/ajax/libs/jquery-cookie/1.4.1/jquery.cookie.min.js"></script>
	<!--カレンダー-->
	{block name="script"}
	{/block}

</head>
<body class="{if $design_name}{$design_name.class_name}{else}designA{/if} {if $data.star_name_en}{$data.star_name_en}{/if}" ontouchstart="">
{if $smarty.server.REMOTE_ADDR != '127.0.0.1'}
{include file="google_analytics.tpl"}
{/if}
<div id="fb-root"></div>
{literal}
<script>(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) return;
  js = d.createElement(s); js.id = id;
	js.async = true;
  js.src = "//connect.facebook.net/ja_JP/sdk.js#xfbml=1&version=v2.8";
  fjs.parentNode.insertBefore(js, fjs);
}(document, 'script', 'facebook-jssdk'));</script>

{/literal}

    <!--[if lt IE 7]>
        <p class="browsehappy">You are using an <strong>outdated</strong> browser. Please <a href="http://browsehappy.com/">upgrade your browser</a> to improve your experience.</p>
    <![endif]-->

    <!-- Add your site or application content here -->
{block name="ranking_head"}

<div class="container ">
	<div class="head">
	</div>
{/block}
	<div class="body">
		<p style="font-size:30px;">Hello world</p>
		{$date.now}/{$star}/{$data.data_type}/{$data.star_name}/{foreach $allRanks as $rank => $sites}{foreach $sites as $rank}*{$rank.site}*{/foreach}{/foreach}/

	</div>
</div>

</body>
</html>
