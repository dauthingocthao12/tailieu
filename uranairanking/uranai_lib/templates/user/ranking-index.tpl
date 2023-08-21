{extends file="main.tpl"}

{block name=seo}
	{if $data.data_type != ''}
		<title>{$seo.date}の{$data.topic_name}ランキング | 12星座占いランキング</title>
	{else}
		<title>12星座占い ランキング | 星占い {$seo.date}の{$seo.topic_or_unsei}</title>
	{/if}
	<meta name="keywords" content="12星座占いランキング,{$seo.date}の運勢,{$seo.topic_or_sougouun},ランキング,占い">
	{if $data.data_type != ''}
		<meta name="description"
			content="12星座占いランキングは様々な占いサイトの情報をまとめてランキング化したサイトです。こちらは{$seo.date}の{$data.topic_name}のランキングページです。数ある占い結果から、あなたにとって良い結果の占いサイトをスムーズにみつけましょう♪">
	{else}
		<meta name="description"
			content="12星座占いランキングは様々な占いサイトの情報をまとめて発信するサービスです。数ある占い結果から、あなたにとって良い結果の占いサイトをスムーズにみつけましょう♪よく当たると評判のサイト・人気占い師によるサイト・少しユニークなサイトまで多種多様な占いサイトから運勢を集計しています。">
	{/if}
{/block}

{block name=body}
	{if $data.rank != NULL}
		<div class="container ranking-index {$data.data_type}">
			<div class="title row text-center">
				<h2 class="font-color" title="星座占い 12星座占いランキング">
					{if $data.date != $date.now}{$data.date}{/if}{if $data.date == $date.now}今日{/if}{if $data.date}の{if $data.data_type != ''}{$data.topic_name}{else}12星座占い{/if}ランキング{/if}
				</h2>
				{include "mainline.parts.tpl"}
			</div>

			{$announcement.ev_banner}
			<div class="text-center">{insert ad_group id="11"}</div>

			<!-- 提案 -->
			{*
            <div class="unsei-suggest font-color text-center">
            <h4>当てはまる方はチェック！</h4>
            {$suggest_msg}
            </div>
						*}
			
			{* LINEbot バナー *}
			<div class="banner_container">
				<a href="https://chataibot.tech/" target="_blank" data-gtm-click="banner_link_pc"><img loading="lazy" src="/user/event_img/renai_AI/LINEAI-pc.png" alt="LINEで恋愛AI相談始めました！"
					class="banner-pc send-analytics banner-img"></a>
				<a href="https://chataibot.tech/" target="_blank" data-gtm-click="banner_link_sp"><img loading="lazy" src="/user/event_img/renai_AI/LINEAI-sp.png" alt="LINEで恋愛AI相談始めました！"
					class="banner-sp send-analytics banner-img"></a>
			</div>

			{if $event_data["event_key"] == "spring"}
				<div class="banner-link"><img loading="lazy" src="{$event_data["banner_path"]}pc.png" alt=""
						class="banner-pc send-analytics modal-start"></div>
				<div class="banner-link"><img loading="lazy" src="{$event_data["banner_path"]}sp.png" alt=""
						class="banner-sp send-analytics modal-start"></div>
			{/if}


				<!--1~3位-->
				<div class="row ranking3-block">

					<div class="col-md-9 ">

						<div class="clearfix spadding rank-day-links font-color">
							{insert 3dates_links mode='rank' topic=$data.data_type prev=$previous_link curr=$config.date_today next=$next_link}
						</div>

						<div class=" clearfix col-md-7 spadding">

							{if $data.data_type == ""}
								<a href="{sitelink mode=detail d=$data.date_num theme=$data.rank[0].en_name star=$data.rank[0].star_num}"
									class="clearfix">
								{else}
									<a href="{sitelink mode=detail topic=$data.data_type d=$data.date_num theme=$data.rank[0].en_name star=$data.rank[0].star_num}"
										class="clearfix" titile="{$data.rank[0].name}星占い">
									{/if}
									<div class="{$data.rank[0].en_name} ">
										<div class="rank1 tabl">
											<div class="tabl-cell table-padding">
												<h3>
													<span class="fc_rank1">
														<span
															class="star_rank{$data.rank[0].num} no1">{$data.rank[0].num}{if $data.data_type != ""}<span
																class="small unit">位</span>{/if}</span>{$data.rank[0].name}
													</span><br>
													<span class="small content-cor fc_rank1">
														{if $data.data_type == ""}
															<span class="small title-indent1">1位の星占いサイト</span>
														{else}
															<span class="small title-indent1">100点の星占いサイト</span>
														{/if}
														{($first_str_con.{$data.rank[0].star_num} == "") ? "0" :
														{$first_str_con.{$data.rank[0].star_num}}}/{$data.sites_count}
													</span>
												</h3>

											</div>
											<div class="rankimg tabl-cell">
												<img loading="lazy" class="rank1 imgsize"
													src="/user/img_re/{$design_name.ev_name}{$data.rank[0].en_name}.png"
													alt="星占い {$data.rank[0].name}" title="星占い {$data.rank[0].name}">
											</div>
										</div>
									</div>
								</a>

						</div>
						<div class=" col-md-5 spadding">

							{if $data.data_type == ""}
								<a href="{sitelink mode=detail d=$data.date_num theme=$data.rank[1].en_name star=$data.rank[1].star_num}"
									class="clearfix" title="星占い {$data.rank[1].name}">
								{else}
									<a href="{sitelink mode=detail topic=$data.data_type d=$data.date_num theme=$data.rank[1].en_name star=$data.rank[1].star_num}"
										class="clearfix" title="星占い {$data.rank[1].name}">
									{/if}
									<div class="{$data.rank[1].en_name} ">
										<div class="rank2 clearfix tabl">
											<div class="tabl-cell table-padding">
												<h4>
													<span class="fc_rank2">
														<span
															class="star_rank{$data.rank[1].num} no2">{$data.rank[1].num}{if $data.data_type != ""}<span
																class="small unit">位</span>{/if}</span>{$data.rank[1].name}
													</span><br>
													<span class="small fc_rank2">
														{if $data.data_type == ""}
															<span class="small title-indent2">1位の星占いサイト</span>
														{else}
															<span class="small title-indent2">100点の星占いサイト</span>
														{/if}
														{($first_str_con.{$data.rank[1].star_num} == "") ? "0" :
														{$first_str_con.{$data.rank[1].star_num}}}/{$data.sites_count}
													</span>
												</h4>
											</div>
											<div class="tabl-cell rankimg">
												<img loading="lazy" class="rank2 imgsize2"
													src="/user/img_re/{$design_name.ev_name}{$data.rank[1].en_name}.png"
													alt="星占い {$data.rank[1].name}" title="星占い {$data.rank[1].name}">
											</div>
										</div>
									</div>
								</a>

								{if $data.data_type == ""}
									<a href={sitelink mode=detail d=$data.date_num theme=$data.rank[2].en_name star=$data.rank[2].star_num}
										class="clerfix" title="星占い {$data.rank[2].name}">
									{else}
										<a href={sitelink mode=detail topic=$data.data_type d=$data.date_num theme=$data.rank[2].en_name star=$data.rank[2].star_num}
											class="clerfix" title="星占い {$data.rank[2].name}">
										{/if}
										<div class="{$data.rank[2].en_name} ">
											<div class="rank3 clearfix aki tabl">
												<div class="tabl-cell table-padding">
													<h4>
														<span class="fc_rank3">
															<span
																class="star_rank{$data.rank[2].num} no3">{$data.rank[2].num}{if $data.data_type != ""}<span
																	class="small unit">位</span>{/if}</span>{$data.rank[2].name}
														</span><br>
														<span class="small fc_rank3">
															{if $data.data_type == ""}
																<span class="small title-indent3">1位の星占いサイト</span>
															{else}
																<span class="small title-indent3">100点の星占いサイト</span>
															{/if}
															{($first_str_con.{$data.rank[2].star_num} == "") ? "0" :
															{$first_str_con.{$data.rank[2].star_num}}}/{$data.sites_count}
														</span>
													</h4>
												</div>
												<div class="tabl-cell rankimg">
													<img loading="lazy" class="rank2 imgsize2"
														src="/user/img_re/{$design_name.ev_name}{$data.rank[2].en_name}.png"
														alt="星占い {$data.rank[2].name}" title="星占い {$data.rank[2].name}">
												</div>
											</div>
										</div>
									</a>
						</div>
						<div class="col-lg-12 adg visible-sm visible-xs">
							<div class="ad-bg modal-ad">
								{if $config.browser_name == "IE" || $config.browser_name == "Edge"}
									{insert ad_group id="6"}
								{else}
									{insert ad_group id="5"}
								{/if}
							</div>
						</div>
						<!--過去バナーsm-->
						<div class="visible-xs visible-sm">
							{insert loginInfo}
						</div>
						<!--過去バナーsm-->
					</div>
					<!--1~3位-->
					<!--カレンダー-->
					<div class="col-md-3 text-center">
						{if $data.sites_count}{include "calendar.parts.tpl"}{/if}
					</div>
					<!--カレンダー-->
				</div>

				<div class="col-md-9">
					{foreach $data.rank as $rank}
						<div class="col-md-4 col-sm-6 spadding">
							{if $data.data_type == ""}
								<a href="{sitelink mode=detail d=$data.date_num theme=$rank.en_name star=$rank.star_num}"
									title="{$rank.num}位 {$rank.name} の星占い">
								{else}
									<a href="{sitelink mode=detail topic=$data.data_type d=$data.date_num theme=$rank.en_name star=$rank.star_num}"
										title="{$rank.num}位 {$rank.name} の星占い">
									{/if}
									<div class="rankother-color otherrank-height">
										<h4 title="{$rank.num}位 {$rank.name} の星占い">
											<span class="star_rank_other">{$rank.num}</span>{$rank.name}
										</h4>
									</div>
								</a>

						</div>
						{if $rank@iteration==6}
							<hr class="ad_sep">
							<div class="col-lg-12 adg">
								<div class="ad-bg modal-ad">
									{if $config.browser_name == "IE" || $config.browser_name == "Edge"}
										{insert ad_group id="6"}
									{else}
										{insert ad_group id="4"}
									{/if}
								</div>
							</div>
							<p class="ranking-message">この{$data.topic_name}ランキングはあくまで集計結果です<br>
								各星座の詳細ページから{if $data.topic_name}{$data.topic_name}の{/if}よい結果のサイトを確認してみてください♪</p>
						{/if}
					{/foreach}
					<hr class="ad_sep">
					{if $data.data_type == ""}
						<!--Twitterアンケート-->
						{if $design_name.sns == "ON"}{include "questionnaire.tpl"}{/if}
						<div class="banner-margin-area">
							{if !$config.isApp}
								<a href="#application-area">
									<img loading="lazy" src="/user/img_re/application-banner-sp.jpg" class="banner-sp width-max">
								</a>
							{/if}
						</div>
						<div class="site_descript">
							<div class="spadding">
								<div class="panel-detail">
									<h4 class="font_bold"><i class="fa fa-smile-o" aria-hidden="true"></i>12星座占いランキングとは…<i
											class="fa fa-smile-o" aria-hidden="true"></i></h4>
									<div class="panel panel-default text-center">
										<div class="panel-body row">
											<div class="margin-content comment text-left col-md-8">
												<p>Webの世界には、12星座占いを扱うサイトがたくさんあり、星座の運勢や順位もバラバラ。<br><br><span
														class="font_bold">「いったいどれを信じたらいいの？」</span><br><br>そんな人のためにあるのがこのサイト！<br>
													たくさんの星占いのサイトから集めた星座の運勢を独自の方法で集計しました。<br><br>上の順位リストをクリックすると、それぞれの星座ページで各星占いサイトでの順位や1週間のランキングの変化がひと目でわかるグラフもチェックできます。
												</p>
											</div>
											<div class="col-md-4 clear-padding">
												<div class="serif">
													良い占い結果だけ信じて毎日ハッピー♪
												</div>
												<div class="clear-padding">
													<img loading="lazy" class="imgs"
														src="/user/img_re/{$design_name.ev_name}{$data.rank[0].en_name}.png"
														alt="星占い {$data.rank[0].name}">
												</div>
											</div>
										</div>
									</div>
								</div>
							</div>
						</div>
					{/if}
					{* 広告 サイトスピード改善のため非表示にします
				<div class="col-lg-12 adg visible-xs visible-sm visible-md visible-lg text-center">
					<div class="ad-bg">
					{if $config.browser_name == "IE" || $config.browser_name == "Edge"}
					{insert ad_group id="1"}
					{else}
					{insert ad_group id="1"}
					{/if}
					</div>
				</div>
*}
					<div class="in-site-links">
						{if $data.data_type != ''}
							<div class="col-sm-6">
								<a href="/">
									<div class="in-site-integrated clearfix">
										<img loading="lazy" class="col-xs-3 in-site-img" src="/user/img_re/defo-banner-character.png"
											alt="星占い 総合運">
										<div class="col-xs-9 in-site-txtsps">
											<h4 class="in-site-title">★総合運星占い</h4>
											<p>総合運の星占い結果はこちら。星座の個別ページではグラフや順位別のサイトリストもご覧いただけます。</p>
										</div>
									</div>
								</a>
							</div>
						{/if}
						{if $data.data_type != 'love'}
							<div class="col-sm-6">
								<a href="/love/">
									<div class="in-site-love clearfix">
										<img loading="lazy" class="col-xs-3 in-site-img" src="/user/img_re/love-banner-character.png"
											alt="星占い 恋愛運">
										<div class="col-xs-9 in-site-txtsps">
											<h4 class="in-site-title">★恋愛運星占い</h4>
											<p>恋愛運の星占い結果はこちら。星座の個別ページではグラフや結果別のサイトリストもご覧いただけます。</p>
										</div>
									</div>
								</a>
							</div>
						{/if}
						{if $data.data_type != 'work'}
							<div class="col-sm-6">
								<a href="/work/">
									<div class="in-site-work clearfix">
										<img loading="lazy" class="col-xs-3 in-site-img" src="/user/img_re/work-banner-character.png"
											alt="星占い 仕事運">
										<div class="col-xs-9 in-site-txtsps">
											<h4 class="in-site-title">★仕事運星占い</h4>
											<p>仕事運の星占い結果はこちら。星座の個別ページではグラフや結果別のサイトリストもご覧いただけます。</p>
										</div>
									</div>
								</a>
							</div>
						{/if}
						{if $data.data_type != 'money'}
							<div class="col-sm-6">
								<a href="/money/">
									<div class="in-site-money clearfix">
										<img loading="lazy" class="col-xs-3 in-site-img" src="/user/img_re/money-banner-character.png"
											alt="星占い 仕事運">
										<div class="col-xs-9 in-site-txtsps">
											<h4 class="in-site-title">★金運星占い</h4>
											<p>金運の星占い結果はこちら。星座の個別ページではグラフや結果別のサイトリストもご覧いただけます。</p>
										</div>
									</div>
								</a>
							</div>
						{/if}
						<div class="col-sm-6">
							<a href="{if $data.data_type != ""}/{$data.data_type}{/if}/ranking-past/">
								<div class="in-site-past clearfix">
									<img loading="lazy" class="col-xs-3 in-site-img" src="/user/img_re/year-banner-character.png"
										alt="星占い 年間・月間">
									<div class="col-xs-9 in-site-txtsps">
										<h4 class="in-site-title">★月間・年間<br>星占いランキング</h4>
										<p>月間・年間の星占いランキングがご覧いただけます。</p>
									</div>
								</div>
							</a>
						</div>
					</div>
				</div>

				{include file='sidebar.tpl'}

			</div>
		{else}
			<div class="container">
				<div class="space">
					<h2 style="text-align: center;">取得した星占い情報がありません。</h2>
				</div>
			</div>
		{/if}
	{/block}