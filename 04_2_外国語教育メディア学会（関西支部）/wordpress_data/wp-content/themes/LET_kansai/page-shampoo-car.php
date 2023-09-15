<?php
/*
    犬の移動美容室ページ用のテンプレート
*/

get_header();
?>
<main class="shampoo-car">
	<section>
		<div class="top-header">
			<div class="header-content">
				<div class="top-left rounded-right top-header-shampoo-car-bg">
					<div class=" top-header-container content-container-md">
						<h2 class="top-header-title"><strong>犬の移動美容室</strong></h2>
					</div>
				</div>
				<?php get_template_part("shared/parts/header", "time-information") ?>
			</div>
		</div>
	</section>

	<section class="section-lb-pink">
		<div class="content-container-md section-padding">
			<h2>犬の移動美容室</h2>
			<div class="col-2">
				<div class="col-text">
					<p>お客様のご自宅まで伺い車内でシャンプーやトリミングをします。<br>お客様にご用意頂くのは駐車場と電源のみです。</p>
					<div class="line-colored text-center">
						<p>通常のトリミング料金+出張料でご利用いただけます。</p>
					</div>
				</div>
				<div class="col-image">
					<img src="<?php echo get_template_directory_uri(); ?>/shared/images/shampoo-car/shampoo-car.jpg" alt="shampoo-car">
				</div>
			</div>
		</div>
	</section>

	<!-- <section class="section-leaf-pink shampoo-car-message">
		<div class="content-container-md section-padding">
			<h2>スタッフからの<span class="break-word">メッセージ</span></h2>
			<div class="col-2">
                <div class="col-image">
                    <img src="<?php echo get_template_directory_uri(); ?>/shared/images/shampoo-car/main-staff-message.jpg" alt="shampoo-car">
               </div>
               <div class="col-text">
                    <p>お店とは違いわんちゃんの生活スタイルを肌で感じることができとても楽しくトリミングやシャンプー施術をさせていただいております。<br>
                        わんちゃんが終わってすぐお家に帰れるというのと施術中もご家族と相談しながら理想のスタイルがつくりやすいのが魅力です。<br>
                        出張というとなんだか大げさに感じるかもしれませんが電源やお湯の準備はすべてスタッフが行いますのでわんちゃんと狂犬病予防だけしてあれば大丈夫なので、お気軽にご相談ください。
                    </p>
               </div>
            </div>
            <div class="read-more increase-padding-top">
                <a href="<?= home_url("shampoo-car") ?>#qa">犬の移動美容室に関するよくある質問<span class="dli-arrow-right"></span></a>
			</div>
		</div>
		<div class="section-padding content-container-md customer-message">
            <h2>お客様の声</h2>
            <div class="col-2">
                <div class="col-image">
                    <img src="<?php echo get_template_directory_uri(); ?>/shared/images/shampoo-car/customer-message.jpg" alt="customer of message">
               </div>
               <div class="col-text">
                    <p>きなこは自分のあまり好きでない人ですとトリミングの後、目がおどおどしたりします。でもそれがなくて落ち着いておりました。とても感謝しております。<br>
                        またお願いしたいと思っております。
                    </p>
               </div>
            </div>
            <div class="col-2">
                <div class="col-image">
                    <img src="<?php echo get_template_directory_uri(); ?>/shared/images/shampoo-car/customer-message-2.jpg" alt="customer of message">
               </div>
               <div class="col-text">
                    <p>長い毛のためか後足で首のあたりをかいていたのが止まりました。<br>
                        適当にカットされきれいにシャンプーされて良かったと思います。<br>
                        とくに、お尻のあたりを短くカットしたのは大変よかったです。<br>
                        ティアラも騒ぐことなくやっていただけましてよかったと思います。<br>
                        有難うございました。
                    </p>
               </div>
            </div>
            <div class="col-2">
                <div class="col-image">
                    <img src="<?php echo get_template_directory_uri(); ?>/shared/images/shampoo-car/customer-message-3.jpg" alt="customer of message">
               </div>
               <div class="col-text">
                    <p>昨日はお世話になりました。<br>
                        他の犬との交わる事の出来ない犬ですのでとても不安でしたが、家迄来ていただく事で、ショコも安心したのでしょう。私達も安心しました。<br>
                        今迄は預けて迎えに行き、出来上がった姿だけを見るだけでしたが、色々と飼主の希望を聞いて下さるしとても嬉しかったです。<br>
                        老齢の為車の運転もいずれは出来なくなると思います。そんな思いの時に‘ワンワンハウス‘を知りほんとに良かったと思っています。<br>
                        これからも宜しくお願い致します。
                    </p>
               </div>
            </div>
        </div>
	</section> -->

	<section class="section-leaf-pink shampoo-car-message">
		<div class="section-padding content-container-md">
			<h2>犬の移動美容室<span class="break-word">(トリミングカー)内部</span></h2>

			<ul class="col-image-2">
				<li>
					<div>
						<img src="<?php echo get_template_directory_uri(); ?>/shared/images/shampoo-car/shampoo-car-2.jpg" alt="軽自動車 ドッグバス">
						<p>軽自動車 ドッグバス</p>
					</div>
				</li>
				<li>
					<div>
						<img src="<?php echo get_template_directory_uri(); ?>/shared/images/shampoo-car/car_inside_kei_aircon.jpg" alt="軽自動車　工アコン">
						<p>軽自動車　工アコン</p>
					</div>
				</li>
			</ul>
		</div>
	</section>

	<section class="section-lb-rt-pink base-cost">
		<div class="section-padding content-container-md">
			<h2>基本料金</h2>
			<div class="sep-25"></div>
			<div class="line-nocolored text-center">
				<p>通常のトリミング料金+出張料でご利用いただけます。</p>
			</div>
			<div class="sep-25"></div>
			<ul>
				<li><span class="text-red">※</span><span class="underline underline-green">トリミングのベーシックコースは犬の移動美容室(トリミングカー)では利用できません。</span></li>
				<li><span class="text-red">※</span>電源の確保ができない場合は別途1500円（発電機使用料と燃料代として）費用が発生致します。</li>
				<li><span class="text-red">※</span>追加料金が発生する場合は事前にご説明いたします。</li>
			</ul>
			<div class="read-more increase-padding-top">
				<a href="<?= home_url("trimming") ?>#course">シャンプーコース、カットコースの料金<span class="dli-arrow-right"></span></a>
			</div>
			<div class="sep-50"></div>
			<dl class="taxi-prices">
				<dt>出張料金</dt>
				<dd>
					<p><b>出張料金価格改定について</b><br>犬の移動美容室（トリミングカー）を多くの飼い主様に利用いただくために今まで出張料金をできる限り低価格に設定をさせていただいておりましたが、昨今の燃料費、人件費、その他経費の高騰により大変心苦しい決断ではありますが出張料金の価格改定をさせていただきます。<br>
						今後もより一層のサービス向上に努めてまいりますので何卒諸般の事情をご賢察頂き、ご理解いただきます様にお願い申し上げます。
					</p>
					<div class="sep-25"></div>
					<div class="shampoo-car-prices">
						<div class="pc">
							<div class="bg-primary-color">地域</div>
							<div class="bg-primary-color">料金</div>
						</div>
						<div>
							<div>前橋市</div>
							<div class="prices" title="料金">2,000円</div>
						</div>

						<div>
							<div>伊勢崎市、渋川市、榛東村、高崎市、玉村町、吉岡町</div>
							<div class="prices" title="料金">2,500円</div>
						</div>

						<div>
							<div>安中市、吾妻郡、太田市、甘楽郡、桐生市、富岡市、沼田市、藤岡市、みどり市</div>
							<div class="prices" title="料金">3,000円</div>
						</div>
					</div>
				</dd>
				<dt>電源をご用意できない場合</dt>
				<dd>
					<p>犬の移動美容室（トリミングカー）をご利用いただく場合はお客様に電源をご用意していただく必要がありますが、電源の確保ができない場合は弊店で発電機を準備致します。<br>
						<span class="underline underline-green"><b>電源をご用意できない場合は別途1500円（発電機使用料と燃料代として）費用が発生致します。</b></span>
					</p>
				</dd>
			</dl>
		</div>
	</section>

	<section class="base-cost" style="background-color:var(--bg-light);">
		<div class="content-container-md section-padding">
			<h2>用意していただくもの</h2>
			<div class="sep-25"></div>
			<ul class="prepare-list">
				<li><span class="text-red">※</span>駐車場</li>
				<li><span class="text-red">※</span>外部電源</li>
				<li class="link"><a href="#qa">電源が無くても可能な場合がございます。詳しくはこちら</a></li>
			</ul>
		</div>
	</section>

	<section class="section-rt-pink">
		<div class="section-padding content-container-md">
			<h2>ご予約の4ステップ</h2>
			<div class="sep-25"></div>

			<dl class="content-flow">
				<div class="content-box noline-colored text-center">
					<dt>STEP1</dt>
					<dd>
						ご利用は予約制となります。まずは電話にてご相談下さい。
						<div class="sep-25"></div>
					</dd>
				</div>

				<div class="triangle-big text-center"></div>

				<div class="content-box noline-colored text-center">
					<dt>STEP2</dt>
					<dd>
						以下の情報をご確認させていただきます。<br>
						<ul class="points-list text-left">
							<li>ワンちゃんの種類</li>
							<li>ワンちゃんの年齢、性別、体重</li>
							<li>ご希望のコース(シャンプーコースまたはカットコース)</li>
							<li>お住いの市町村</li>
						</ul>
					</dd>
				</div>

				<div class="triangle-big text-center"></div>

				<div class="content-box noline-colored text-center">
					<dt>STEP3</dt>
					<dd>
						STEP2でお伺いした情報から、基本料金をお伝えいたします。<br>
						ご予約希望の場合は以下の内容をご確認させていただきます。
						<ul class="points-list text-left">
							<li>駐車場の有無</li>
							<li>外部電源の有無</li>
							<li>混合ワクチンの1年以内の接種状況</li>
							<li>狂犬病の予防接種の確認</li>
						</ul>
					</dd>
				</div>

				<div class="triangle-big text-center"></div>

				<div class="content-box noline-colored text-center">
					<dt>STEP4</dt>
					<dd>
						STEP3の確認が出来ましたら、ご予約に必要な情報をお伺いいたします。
						<ul class="points-list text-left">
							<li>出張可能な日程</li>
							<li>ご希望のお日にち</li>
							<li>お客様のお名前</li>
							<li>住所</li>
							<li>電話番号</li>
						</ul>
					</dd>
				</div>
			</dl>

			<div class="sep-50"></div>

			<div class="content-box line-nocolored text-center main-text">
				ご予約は以上です！<br>当日は店舗を出発前にお電話してからご自宅に出張いたします。
				<div class="sep-25"></div>
				<ul>
					<li>
						営業時間 : 9:00～19:00
					</li>
					<li>
						定休日 : 毎週木曜日
					</li>
					<li>
						電話番号 : <a href="tel:0272345400" class="text-color-black">TEL 027-234-5400</a>
					</li>
				</ul>
			</div>
		</div>
	</section>

	<a name="qa"></a>
	<section class="section-padding" style="background-color: var(--bg-light);">
		<div class="content-container-md">
			<h2>よくある質問</h2>
			<div class="sep-50"></div>

			<dl class="qa-list">
				<?php
				$qa_entries = qa_list("shampoo-car");
				if (count($qa_entries) === 0) {
					echo "<p class='text-center'>情報がありません</p>";
				}
				foreach ($qa_entries as $entry) {
					/** @var WP_Post $entry */
					echo '<div class="qa-group">';
					echo "<dt>{$entry->post_title}</dt>";
					echo "<dd>{$entry->post_content}";
					echo '<div class="post-thumbnail">';
					echo get_the_post_thumbnail($entry);
					echo '</div>';
					echo "</dd>";
					echo '</div>';
				}
				?>
			</dl>
		</div>
	</section>

	<?= get_template_part("shared/parts/common", "banners") ?>
</main>

<?php
get_footer();
