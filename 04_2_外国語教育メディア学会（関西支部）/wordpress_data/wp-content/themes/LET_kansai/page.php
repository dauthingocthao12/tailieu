<?php 
/* 
Template Name:テストです
* Template Post Type: post, page
*/
?>

<?php get_header();
?>
<div class="page-content">

<?php get_sidebar(); ?>

<div class="page-main">
<?php 

	
	if ( have_posts() ) {
		if(is_page( 'HOME' )){
		//del okabe start 2017/09/12
		//	print('<ol class="detail-menu">');
		//		print('<li><a href ="#greeting">支部長挨拶</a></li>');
		//		print('<li><a href ="#officer-list">関東支部役員一覧</a></li>');
		//	print('</ol>');
		//del okabe end 2017/09/12
		//add okabe start 2017/09/12
		} else if(is_page( 'greetings')) {
			print('<ol class="detail-menu">');
				print('<li><a href ="#greeting">ごあいさつ</a></li>');
				print('<li><a href ="#about">LETとは</a></li>');
			print('</ol>');
		//add okabe end 2017/09/12
		}else{
			print ('<h2>' . get_the_title() . '</h2>');
			print('<li><a href ="#greeting">関西テスト</a></li>');
		}

			the_post();
			the_content();

		//add okabe start 2017/09/12
		if(is_page( 'greetings' )){


print('<div class="about">');
//スラッグ名でリンクを指定 固定ページ出力
$args = array(
  'pagename'        => 'about'
);
$the_query = new WP_Query($args);
if ( $the_query->have_posts() ) {   
    $the_query->the_post();
	print "<h2 class='title clear' id='about'>".get_the_title()."</h2>";
	the_content();
}
wp_reset_postdata();//必須
print('</div>');


		}
		//add okabe end 2017/09/12
	}?>

</div>
</div>
	
<?php get_footer(); ?>
