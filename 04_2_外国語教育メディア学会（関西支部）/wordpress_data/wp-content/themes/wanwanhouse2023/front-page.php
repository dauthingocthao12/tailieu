<?php
get_header();
?>
	<section>
		<div class="top-header">
			<div class="header-content">
				<div class="top-left">
					<div class="top-slides">
						<div class="slide-image-1">
							<div class="content-container-md section-padding">
								<div class="top-title">
									<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-grooming-text-2.png" alt="犬の移動美容室">
								</div>
								<div class="top-text-image">
									<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-grooming-text.png" alt="犬の移動美容室">
								</div>
							</div>
						</div>
						<div class="slide-image-2">
							<div class="content-container-md section-padding">
								<div class="top-title">
									<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-mobile-hairdresser-text-2.png" alt="犬の移動美容室">
								</div>
								<div class="top-text-image">
									<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-mobile-hairdresser-text.png" alt="犬の移動美容室">
								</div>
							</div>
						</div>
						<div class="slide-image-3">
							<div class="content-container-md section-padding">
								<div class="top-title">
									<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-dog-hotel-text-2.png" alt="ドッグホテル">
								</div>
								<div class="top-text-image">
									<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-dog-text.png" alt="ドッグホテル">
								</div>
							</div>
						</div>
						<div class="slide-image-4">
							<div class="content-container-md section-padding">
								<div class="top-title">
									<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-pet-taxi-text-2.png" alt="ペットタクシー">
								</div>
								<div class="top-text-image">
									<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-pet-taxi-text.png" alt="ペットタクシー">
								</div>
							</div>
						</div>
						<div class="slide-image-5">
							<div class="section-padding">
								<div class="top-title top-netshop-layout">
									<div class="net-shop-title-pc">
										<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-net-shop-text-2.png" alt="ネットショップ">
									</div>
									<div class="net-shop-title-sp">
										<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-net-shop-text-sp.png" alt="ネットショップ">
									</div>
									<div class="increase-space-top">
										<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/rakuten-net-shop.png" alt="Rakutenネットショップ">
									</div>
									<div class="increase-space-top">
										<img src="<?= get_template_directory_uri() ?>/shared/images/index/yahoo-net-shop.png" alt="Yahooネットショップ">
									</div>
								</div>
								<div class="top-text-image">
									<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-net-shop-text.png" alt="ネットショップ">
								</div>
							</div>
						</div>
					</div>
				</div>
				<?php get_template_part("shared/parts/header", "time-information") ?>
			</div>
		</div>
		<div class="top-image-content content-container-md ">
			<div class="image-left">
				<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-mobile-hairdresser.jpg" alt="犬の移動美容室">
			</div>
			<div class="image-center">
				<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-dog.jpg" alt="ワンちゃん">
			</div>
			<div class="image-right">
				<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/header-walking.jpg" alt="散歩">
			</div>                   
		</div>
	</section>

	<main>
		<div class="sep-50"></div>
		<section class="section-leaf-pink">
			<div class="content-container-md section-padding">
				<h2>サービス</h2>
				<ul class="service-content">
					<li class="card">
						<div class="card-image-top">
							<a href="<?php echo home_url("trimming") ?>" title="トリミング">
								<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/service-grooming.jpg" alt="トリミング">
							</a>
						</div>
						<a href="<?php echo home_url("trimming") ?>" title="トリミング" class="card-body bg-brown">
							<h3>トリミング</h3>
							<span class="dli-arrow-right text-light"></span>
						</a>
						<p class="increase-space">小さな幼犬から10歳以上のシニア犬もお預かり。トリミングが苦手な愛犬にもストレスを与えず優しく丁寧に。</p>
					</li>
					<li class="card increase-spacing-card">
						<div class="card-image-top">
							<a href="<?php echo home_url("shampoo-car") ?>" title="犬の移動美容室">
								<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/service-mobile-hairdresser.jpg" alt="犬の移動美容室">
							</a>
						</div>
						<a href="<?php echo home_url("shampoo-car") ?>" title="犬の移動美容室" class="card-body bg-blue">
							<h3>犬の移動美容室</h3>
							<span class="dli-arrow-right text-light"></span>
						</a>
						<p class="increase-space">怖がりな愛犬やシニア犬、また飼い主様のご都合でトリミングサロンに行けない時は、お電話一本でトリミングカーがご自宅へ。</p>
					</li>
					<li class="card">
						<div class="card-image-top">
							<a href="<?= home_url("hotel")?>" title="ドッグホテル">
								<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/service-dog-hotel.jpg" alt="ドッグホテル">
							</a>
						</div>
						<a href="<?= home_url("hotel")?>" title="ドッグホテル" class="card-body bg-pink">
							<h3>ドッグホテル</h3>
							<span class="dli-arrow-right text-light"></span>
						</a>
						<p class="increase-space">お仕事やご旅行、お引っ越しなどで一時的に愛犬のお世話ができない時は、ドッグホテルをお気軽にご利用ください。トリミングやペットタクシーと一緒のご利用でさらにお得！</p>
					</li>
					<li class="card">
						<div class="card-image-top">
							<a href="<?php echo home_url("pet-taxi") ?>" title="ペットタクシー">
								<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/service-pet-taxi.jpg" alt="ペットタクシー">
							</a>
						</div>
						<a href="<?php echo home_url("pet-taxi") ?>" title="ペットタクシー" class="card-body bg-purple">
							<h3>ペットタクシー</h3>
							<span class="dli-arrow-right text-light"></span>
						</a>
						<p class="increase-space">病院やサロン、ご友人のお家など、愛犬の快適な移動をお約束します。ペット輸送を目的とした国土交通省陸運局届出済です。</p>
					</li>
					<li class="card">
						<div class="card-image-top">
							<a href="<?= home_url("net-shop")?>" title="ネットショップ">
								<img src="<?php echo get_template_directory_uri(); ?>/shared/images/index/service-netshop.jpg" alt="ネットショップ">
							</a>
						</div>
						<a href="<?= home_url("net-shop")?>" tilte="ネットショップ" class="card-body bg-olive">
							<h3>ネットショップ</h3>
							<span class="dli-arrow-right text-light"></span>
						</a>
						<p class="increase-space">愛犬との生活をもっと豊かに！犬用グッズ専門ネットショップ「ドッグサポートアエコム」は、店舗スタッフ自身の目で確かめ、厳選した品揃えでご来店をお待ちしています。</p>
					</li>
				</ul>
			</div>
		</section>

		<section class="home-posts section-rt-pink">
			<div class="section-padding content-container-md">
				<a name="news"></a>
				<div class="flex-between flex-box">
					<h2 class="h2-span-right">お知らせ</h2>
					<ul class="post-categories">
					<?php
						$categories = list_categories();
						foreach($categories as $slug => $name) {
							echo "<li><a href='?c=$slug#news' class='post-category-label $slug'>$name</a></li>\n";
						}
					?>
					</ul>
				</div>

				<?php
				$args = [
					'post_type' => 'post',
					'post_status' => 'publish',
					'posts_per_page' => 5,
					'orderby' => 'date',
					'order' => 'DESC',
				];
				if($_GET['c']!="all") {
					$args['category_name'] = $_GET['c'];
				}
				// echo '<pre>'; var_export( $args ); echo '</pre>'; // DBG

				$custom_query = new WP_Query ( $args );
				if($custom_query->have_posts()) {
					?>
					<ul class="posts-list">
						<?php while($custom_query->have_posts()) {
							$custom_query->the_post();
							$category = get_the_category()[0];
							$slug = $category->slug;
							$cate = $category->name;
							?>
							<li class="post-entry">
								<?= post_category_label($slug, $cate) ?>
								<span class="post-date"><?= get_the_date(POST_DATE_FORMAT) ?></span>
								<a href="<?= get_the_permalink() ?>" class="post-title"><?= get_the_title() ?></a>
							</li>
							<?php
							}
							wp_reset_postdata();
						?>
					</ul>
					<div class="more-block read-more">
						<a href="<?= home_url(BLOG_SLUG) ?>">more<span class="dli-arrow-right"></span></a>
					</div>
					<?php
				}
				else {
					?>
					<p>記事がありません。</p>
					<?php
				}
				?>
			</div>
		</section> <!-- end home-posts -->

		<section class="section-lb-white">
			<div class="section-image-1"></div>
			<div class="section-padding message-content content-container-md">
				<h2>メッセージ</h2>
				<p class="message-text increase-space">当社のホームページをご覧いただきありがとうございます。<br>
					1980年に創業今年43年を迎える当社は群馬県前橋市で自家繁殖の犬を直接販売する犬専門のペットショップとして創業いたしました。思えば時代の変化と共に飼い主様の愛犬に対する意識にも大きな変化がみられます。当社もその変化に対応するため試行錯誤を重ね自己変革に取り組んでまいりました結果現在では創業当時とは事業内容も大きく変り、犬の繁殖、生体販売は中止し犬のトリミング、犬のペットホテル、犬の移動美容室（トリミングカー）、犬に関する商品のネット販売(ショップ名ドッグサポートアエコム)に加え2022年3月より国土交通省陸運局許可営業ナンバーを所得しペット輸送を目的としたペットタクシー事業を開始いたしました。<br>
					当社は犬だけに特化した会社として今後も少しでも飼い主様と愛犬との生活が豊かで（選択の提供）より良い（今より少しでも良くなる商品、サービスの提供）ものとなる様にサポートし社会の中で必要とされる会社となることを目指しております。<br>
					今回　各事業（カテゴリー）に対し基本的な考え方、創意工夫、努力していることなどを具体的に当社の思いをお伝えできるように当社ホームページを更新いたしました。<br>
					当社に関心を持って頂きました方々のお役に立てば幸いと存じます。<br>
				</p>
				<p class="text-right font-sans-serif">	2023年2月吉日<br>有限会社ワンワンハウス<br>代表取締役　樺澤功生</p>
			</div>
		</section>

		<section class="contact-area">
			<div class="content-container-md  section-padding">
				<h2>お問い合わせ</h2>
				<div class="contact-content">
					<div class="mail-contact">
						<a href="<?php echo home_url('contact') ?>"><i class="fa fa-envelope icon"></i></a>
						<div class="contact-text">
							<a href="<?php echo home_url('contact') ?>">メールでのお問い合わせ</a>
							<div class="border-bottom"></div>
						</div>
					</div>
					<div class="tel-contact">
						<a href="tel:0272345400"><i class="fa fa-phone icon"></i></a>
						<div class="contact-text">
							<a href="tel:0272345400">TEL<span class="text-lg"> 027-234-5400</span></a>
							<p class="fs-small">営業時間 : 9:00~19:00／定休日 : 毎週木曜日</p>
							<div class="border-bottom"></div>
						</div>
					</div>
				</div>
			</div>
		</section>

		<section class="section-image-2">
			<div class="content-container-md  section-padding"></div>
		</section>

		<section class="social-media-content">
			<div class="content-container-md  section-padding flex-box">
				<div class="col-lg">
					<div class="front-sns-logo">
						<a href="https://www.instagram.com/wanwanhouse/" target="_blank" title="INSTAGRAM">
							<i class="fa fa-instagram fa-2x"></i>
						</a>
					</div>
					<?php echo wanwanhouse_instagram(6) ?>
				</div>
				<div class="front-facebook">
					<div class="front-sns-logo">
						<a href="https://www.facebook.com/%E3%83%AF%E3%83%B3%E3%83%AF%E3%83%B3%E3%83%8F%E3%82%A6%E3%82%B9-%E5%89%8D%E6%A9%8B-894466287315926" target="_blank" title="FACEBOOK">
							<span class="fa-stack">
								<i class="fa fa-circle fa-stack-2x"></i>
								<i class="fa fa-facebook fa-stack-1x fa-inverse"></i>
							</span>
						</a>
					</div>
					<iframe
						src="https://www.facebook.com/plugins/page.php?href=https%3A%2F%2Fwww.facebook.com%2F%E3%83%AF%E3%83%B3%E3%83%AF%E3%83%B3%E3%83%8F%E3%82%A6%E3%82%B9-%E5%89%8D%E6%A9%8B-894466287315926&tabs=timeline&width=290&height=500&small_header=false&adapt_container_width=true&hide_cover=false&show_facepile=true&appId"
						width="290" style="border:none;overflow:hidden;" scrolling="no"
						frameborder="0" allowfullscreen="true"
						allow="autoplay; clipboard-write; encrypted-media; picture-in-picture; web-share">
					</iframe>
				</div>
			</div>
		</section>
	</main>

<?php
get_footer();
