<?php

define("POST_DATE_FORMAT", "Y.m.d");
define("BLOG_SLUG", "blog"); // お知らせ・ブログなど


add_theme_support("post-thumbnails");

/**
 * ポストの可能のカテゴリーを出す
 *
 * @return array
 */
function list_categories()
{
	$order = [
		'news',
		// 'goods',
		'event',
		'blog',
	];

	$avail_categories = get_categories([
		'hide_empty' => false,
	]);

	// echo '<pre>'; var_export( $avail_categories ); echo '</pre>'; // DBG

	$categories = [
		'all' => 'すべて',
	];

	foreach($order as $cate_slug) {
		foreach($avail_categories as $cate) {
			if($cate->slug===$cate_slug) {
				$categories[$cate_slug] = $cate->name;
			}
		}
	}

	return $categories;
}


/**
 * ポストのカテゴリーのラベルを作成
 *
 * @param string $slug
 * @param string $name
 * @return string HTML
 */
function post_category_label($slug, $name)
{
	return "<span class='post-category-label $slug'>$name</span>";
}


/**
 * ブログのカテゴリーのリンクを作る
 *
 * @return string HTML
 */
function posts_categories_links()
{
	$html = "<ul class='post-categories clearfix'>";

	$categories = list_categories();
	foreach($categories as $slug => $name) {
		if($slug=="all") $slug_url = home_url(BLOG_SLUG);
		else $slug_url = home_url("category/$slug");
		$html .= "<li><a href='$slug_url' class='post-category-label $slug'>$name</a></li>\n";
	}

	$html .= "</ul>";

	return $html;
}


/**
 * Q&Aの一覧を作る
 *
 * @param string $category_
 * @return WP_object[]
 */
function qa_list($category_)
{
	$results = [];

	$args = [
    'post_type' => 'post_qa',          // custom post type
    'post_status' => 'publish',
    'posts_per_page' => -1,
    // 'orderby' => 'post_title',
    // 'order' => 'DESC',
    'tax_query' => [[     // this return posts that have a taxonomy with slug 'xxx'
			'taxonomy' => 'loai_qa',
			'field'    => 'slug',
			'terms'    => $category_
		]],
	];

	$custom_query = new WP_Query( $args );
	$results = $custom_query->get_posts();

	return $results;
}


function trimming_get_posts( $query ) {
	// echo '<pre>'; var_export( $query->query_vars ); echo '</pre>'; // DBG

	if($query->is_main_query() && $query->query_vars['post_type']=='trimming_menu' ) {
		$query->set('posts_per_page', -1);
	}
	return $query;
}
add_filter( 'pre_get_posts', 'trimming_get_posts' );


/* INSTA 関係 */


/**
 * URLにトークンを追加する
 *
 * @param string $start_
 * @return string
 */
function insta_make_api_url($start_)
{
	return $start_."&access_token=EAAMoOSmTMYABANW81AvqigsD70mYCeWUGVOBrSkX8JDYi8izeE740RLj0niTjBblmIV0BkIXtjEonehpGWqGKa1QNZAkXVB5fhZBfm9E5PL7qQKCOLaAUySvklN6TxZAXzYbRpZAG6q4j20ZBHHmFVwTqDuQobswvUvPuW6QCO5E6eZB6BhZByE";
}


/**
 * ｃURLでAPIにリクエストする
 *
 * @param string $URL_
 * @return string
 */
function insta_get_contents($URL_){
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_HEADER, 0);
	curl_setopt($ch, CURLOPT_POST, 0);
	curl_setopt($ch, CURLOPT_URL, $URL_);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
	$data = curl_exec($ch);
	curl_close($ch);
	return $data;
}


/**
 * INSTAデータ（一覧）をパースする
 * (API結果を整理して、使いやすいように)
 *
 * @param array $feed_
 * @param int $count_
 * @return array
 */
function insta_parse($feed_, $count_)
{
	$entries = [];

	foreach($feed_ as $f) {
		if($f['media_type']=='VIDEO') {
			$entries[] = [
				'permalink' => $f['permalink'],
				'thumbnail' => $f['thumbnail_url']
			];
		}
		else {
			// get thumbnails for `CAROUSEL_ALBUM` and `IMAGE`
			$entries[] = [
				'permalink' => $f['permalink'],
				'thumbnail' => $f['media_url'],
			];
		}

		// limit check
		if(count($entries)>=$count_) break;
	}

	return $entries;
}


/**
 * 画像リストを作る
 *
 * @param array $items_ insta_parse()の結果
 * @return string HTML
 */
function insta_show($items_)
{
	$html = '<ul class="instagram-list increase-space">';
	foreach($items_ as $item) {
		// $pic = "<img src='{$item['thumbnail']}' class='image-responsive'>";
		$style = "style='background: url({$item['thumbnail']}) center center no-repeat; background-size: cover;'";
		$html .= "<li><a class='item-box' href='{$item['permalink']}' target='_blank' $style></a></li>";
	}
	$html .= '</ul>';

	return $html;
}


/**
 * INSTAを読み込んで、表示する
 *
 * @param int $pics_count_
 * @return string
 */
function wanwanhouse_instagram($pics_count_)
{
	$html = ""; // return

	// graph api url
	$api_url_start  = "https://graph.facebook.com/v15.0/";
	// instagram business ID
	$api_url_start .= "17841405525490327";
	// fields
	$api_url_start .= "/?fields=media{id,media_type,media_url,thumbnail_url,permalink}";
	// 3rd access token
	$api_url = insta_make_api_url($api_url_start);
	// echo '<pre>'; echo( $api_url ); echo '</pre>'; // DBG

	$response = insta_get_contents($api_url);
	// echo '<pre>'; var_export( $response ); echo '</pre>'; // DBG
	if($response===false) return "";

	$json = json_decode($response, true);
	// echo '<pre>'; var_export( $json ); echo '</pre>'; // DBG

	$feed = $json['media']['data'];
	if(count($feed)>0) {
		$items = insta_parse($feed, $pics_count_);
		// echo '<pre>'; var_export( $items ); echo '</pre>'; // DBG
		$html = insta_show($items);
	}
	else {
		// nothing?
	}

	return $html;
}
