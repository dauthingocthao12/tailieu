{extends file="main.tpl"}
{block name=seo}
	{nocache}
	{if $data.date == $date.now}
		<title>{$data.star_name_kanji}{if $data.data_type != ''}の{$data.topic_name}{/if}</title>
		<meta name="keywords" content="12星座占いランキング,{$data.star_name},星座占い,まとめ,{$seo.topic_or_unsei},ランキング,{$data.date}">
		<meta name="description"
			content="各サイトから取得した12星座占いの{$seo.topic_or_empty}ランキング情報を集計し、{$seo.date}の『{$data.star_name}』の結果をまとめたものを表示しています。ここ１週間の順位変化のグラフも表示しています。閲覧は無料です。ユーザー登録するとメールで毎日結果をお知らせ♪集計サイトも随時更新中!!">
	{/if}
	{/nocache}
	<!--OGP START-->
	{include file="ogp.tpl" title="随時更新中！"
												des="各サイトから取得した12星座占いの{$seo.topic_or_empty}ランキング情報を集計し、{$seo.date}の『{$data.star_name}』の結果をまとめたものを表示しています。ここ１週間の順位変化のグラフも表示しています。閲覧は無料です。ユーザー登録するとメールで毎日結果をお知らせ♪集計サイトも随時更新中!!"}
	<!--OGP END-->
{/block}



{block name="body"}
	{if $data.current_rank.num != 0 && $updown_rank != ""}

		<!-- モーダル・ダイアログ -->
		{include file='bookmark_modal.tpl'}
		<!-- モーダル・ダイアログ -->

		<div class="ranking-details margin-content {$data.data_type}">
			<div class="title row text-center font-color">
				{if $data.data_type == ""}
					<h2 class="font-color">{$data.star_name_kanji}の<br class="visible-xs">12星座占いランキング</h2>
				{else}
					<h2 class="font-color">{$data.star_name_kanji}の<br class="visible-xs">{$topic_Jp}ランキング</h2>
				{/if}
				{include "mainline.parts.tpl"}
			</div>
			{$announcement.ev_banner}

			<div class="text-center">{insert ad_group id="11"}</div>
			<!-- バナー-->
			<div style="">
			</div>

			{if !$config.isApp}
				<a href="#application-area">
					<img loading="lazy" src="/user/img_re/application-banner-sp.jpg" class="banner-sp width-max">
				</a>
			{else}
				{insert loginInfo}
			{/if}
			<!-- バナーend-->

			<div class="text-center">
				<div class=" row">

					<div class=" col-md-9">
						<div class="clearfix rank-day-links">
							{insert 3dates_links mode='detail' topic=$data.data_type prev=$previous_link curr=$config.date_today next=$next_link star=$details_star}
						</div>
						<div class="spadding">
							<div class="panel-detail clearfix">
								{if $data.data_type == "love"}
									<p>恋してなくても、してる人もなんとなく毎日見てしまう恋愛運♪<br>このページでは{$data.star_name}の恋愛運に関する占いサイトをまとめてみました。<br>「今日は好きな人と会えるかな」「今日は素敵な出会いがあるかな」<br>恋愛運のよい占いサイトを見て、皆様がわくわくで楽しい1日を過ごせますように♪
									</p>
								{elseif $data.data_type == "work"}
									<p>日常生活で欠かせない「仕事」<br>毎日仕事をする中で、色々な悩みや不安が出てきますよね…。<br>そこで、{$data.star_name}の仕事運の情報に関する占いサイトをまとめてみました。<br>「今日は大事な仕事が…」「あの仕事今日中に終わるかな…」など<br>お仕事の不安があるときに、仕事運の良い結果の占いサイトを見てみるとよいかもしれません。<br>良い占い結果をみて、少しでもモチベーションを高くして頑張りましょう♪
									</p>
								{elseif $data.data_type == "money"}
									<p>日常生活で必要不可欠なお金。金運はいつでも気になる運勢ですね。<br>そこで、{$data.star_name}の金運情報をまとめた占いサイトをまとめてみました。<br>「欲しいものがあるけど、悩むなぁ…」など、お金に関して悩んだ時、<br>いろんな占いサイトの{$data.star_name}の金運を見てみて参考にするのも面白いかもしれません♪
									</p>
								{/if}
								{if $data.data_type == ""}

									{* 猫の日イベント用モーダル起動ボタン *}
									{if $event_data["event_key"] == "cat"}
										<div class="modal-btn modal-start send-analytics">他の猫は？</div>
									{/if}

									<div><span class="word-break">{$data.date}の</span><span
											class="word-break">{$data.star_name}の運勢は…</span><span class="dtl-rank"><span
												class="detail-ranking">{$data.current_rank.num|default:'?'}位</span>です！</span>
									</div>
									<div class""><a href="#sns_btn_adjust">順位の詳細をチェック!</a></div>
									<div class="star-comment text-center">
										<div class="panel-body ">
											<div class="col-sm-8 margin-content comment text-left">
												{$seiza_comment}
											</div>
											<div class="col-sm-4">
												<p class="speech-bubbles">{$custom_message->of("SPEECH_BUBBLES")}</p>
												<img loading="lazy" class="starimg"
													src="/user/img_re/{$design_name.ev_name}{$data.star_name_en}.png"
													alt="星座占いサイトの{$data.star_name}キャラクター">
											</div>
										</div>
									</div>

									{if $event_detail_data["event_key"] == "spring"}
									{* イベント用コンテンツ *}
										<div class="season-section">
											<div class="season-img">
													<img loading="lazy" class="starimg event-detail-img"
													src="{$event_detail_data['icon_path']}">
											</div>
											<div class="season-text">
												{$event_detail_data['message']}
											</div>
										</div>
									{{/if}}

								{else}
									<div class="tabl topic-dtl">
										<div class="tabl-cell cell1">
											<span class="word-break">{$data.date}の</span><span
												class="word-break">{$data.star_name}の{$topic_Jp}は…</span>
											<span class="dtl-rank"><span
													class="detail-ranking">{$data.current_rank.num|default:'?'}位</span>です！</span>
										</div>
										<div class="tabl-cell cel2">
											<img loading="lazy" class="starimg"
												src="/user/img_re/{$design_name.ev_name}{$data.star_name_en}.png"
												alt="{$data.star_name}">
										</div>
									</div>
									{if $data.data_type == "love"}
										<p class="message">
											交際中でも片思い中でも、良い点数の星座占いサイトを見てドキドキハッピーな１日を過ごせますように♪<br>明日も{$data.star_name}の運勢をチェックしてね！</p>
										<div class""><a href="#sns_btn_adjust">気になる{$data.star_name}の恋愛運の詳細をチェック!</a></div>
									{elseif $data.data_type == "work"}
										<p class="message">良い点数の星座占いサイトを見て少しでもモチベーション高く頑張れますように♪</p>
										<div class""><a href="#sns_btn_adjust">{$data.star_name}の仕事運の詳細をチェック!</a></div>
									{else}
										<div class""><a href="#sns_btn_adjust">順位の詳細をチェック!</a></div>
									{/if}


								{/if}
							</div>
						</div>
					</div>
					<!--　カレンダー -->
					<div class=" col-md-3" id="calendar">
						{include "calendar.parts.tpl"}
					</div>
					<!--　カレンダー　-->
				</div>
				<div class=" col-md-9" id="detail-wrapper">
					<a id="sns_btn_adjust"></a>
					{insert loginInfo}
					<div class="row graph-space margin-content">
						<h3 id="weekly-graph" class="font-color section-title">一週間の運勢変化グラフ</h3>
						<div class="graph-comment font-color">
							<p>{$data.date}の{$data.star_name}の運勢は、<br class="visible-xs">前日の日付より<br
									class="visible-xs">{$updown_rank}</p>
						</div>
						<div class="graph col-md-10 col-md-offset-1 font-color">{$graph_data}</div>
					</div>
					<hr class="ad_sep">
					<div class="col-lg-12 adg">
						<div class="ad-bg modal-ad">
							{if $config.browser_name == "IE" || $config.browser_name == "Edge"}
								{insert ad_group id="6"}
							{else}
								{insert ad_group id="5"}
							{/if}
						</div>
					</div>
					<hr class="ad_sep">
					<!--
					<div class="salvation margin-content">
						<div class="btm-margin-L">救済</div>
						<div class="site-list">リスト</div>
					</div>
					-->
					<!--Twitterアンケート-->
					{if $design_name.sns == "ON"}{include "questionnaire.tpl"}{/if}

					<!-- 提案 -->
					<div class="unsei-suggest font-color text-center">
						<h4>当てはまる方はこちらもチェック！</h4>
						{$suggest_msg}
					</div>

					<div class="ranking-all margin-content ">
						<h3 id="sitelist" class="section-title">各サイト別順位表</h3>
						<p class="btm-margin-L font-color">各星占いのサイトで{$data.star_name}が何位だったかご覧になることができます。</p>
						<div class="clearfix">
							<div class="alert alert-info">
								<p>サイト名の先頭に「※」が付いている星座占いサイトは過去の情報が閲覧出来ないため、最新の情報へリンクしています。</p>
							</div>
							{if $data.data_type != ""}
								<div class="col-md-8 text-left"> ※サイト独自に評価した得点で表示しています。</div>
							{/if}
							<div class="col-md-4  site-count alert alert-info">
								{$data.sites_count|default:0} 件の星座占いサイトから集計しました。
							</div>
						</div>

						{foreach $allRanks as $rank => $sites}
							{if $data.data_type == ""}
								<div class="panel panel-default panel-rank-details-sites text-left">
								{else}
									<div class="panel panel-warning panel-rank-details-sites text-left">
									{/if}
									<div class="panel-heading{if $sites@iteration==1} open{/if}" data-rank="{$rank}">
										{if $data.data_type == ""}
											<span class="rank-number">{$rank} 位</span> {$sites|count}件
										{else}
											{if $rank == 100}
												<span class="rank-number">{$rank}点</span> {$sites|count}件
											{elseif $rank == 0}
												<span class="rank-number">{10}点未満</span> {$sites|count}件
											{else}
												<span class="rank-number">{$rank}点台</span> {$sites|count}件
											{/if}
										{/if}

									</div>
									<div class="panel-body" {if $sites@iteration>1}style="display:none;" {/if}>
										<ul class="ranking-sites table">
											{assign var="random_ad_index" value=array_rand($sites, 1)}
											{foreach $sites as $k => $rank}
												<li class="table-row">
													<div class="table-cell">
														{if $config.isApp or $user}
															<a data-toggle="modal"
																onclick="bookmark('{$rank.site_url}','{$rank.site|strip:"&nbsp;"}',{$rank.site_id},'app_user'); ">{$rank.site}</a>
														{else}
															<a data-toggle="modal"
																onclick="bookmark('{$rank.site_url}','{$rank.site|strip:"&nbsp;"}',{$rank.site_id});">{$rank.site}</a>
														{/if}
													</div>

													<div class="site-rank-data table-cell">
														{if $sites_ranking[$rank.site_id]}
															<a href="{sitelink mode="site-description/{$rank.site_id}" }">
																<div class="visible-sm visible-md visible-lg">
																	<!-- pc + -->
																	{insert siteEvaluationStars evaluation=$sites_ranking[$rank.site_id].evaluation_average}
																	{$sites_ranking[$rank.site_id].evaluation_average|number_format:1}
																	<i class="fa fa-commenting-o fa-flip-horizontal"></i>
																	{$sites_ranking[$rank.site_id].comments_count}
																</div>
																<div class="visible-xs">
																	<!-- phone -->
																	<span class="fa-stack">
																		<i class="fa fa-stack-2x fa-star"></i>
																		<span
																			class="fa-stack-1x evaluation-number">{$sites_ranking[$rank.site_id].evaluation_average|number_format:1}</span>
																	</span>
																	<small>(<i class="fa fa-commenting-o fa-flip-horizontal"></i>
																		{$sites_ranking[$rank.site_id].comments_count})</small>
																</div>
															</a>
														{else}
															<a href="{sitelink mode="site-description/{$rank.site_id}" }"
																class="site-description-link"><span class="hidden-xs">このサイトを</span>評価する</a>
														{/if}
													</div>
												</li>

												{if $k == $random_ad_index}
													<li class="table-row">
														<div class="table-cell">
															<span>{insert ad_group id="7"}</span>
														</div>
														<div class="table-cell tecen">
															<span class="label label-info">PR</span>
														</div>
													</li>
												{/if}

											{/foreach}
										</ul>
									</div>
								</div>
							{/foreach}
						</div>
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
						{if $data.data_type == "love"}
							<a class="link_style" href="{sitelink mode="work/{$data.star_name_en}"
					}"}>{$data.star_name}の仕事運をチェック♬</a>
						{elseif $data.data_type == "work"}
							<a class="link_style" href="{sitelink mode="money/{$data.star_name_en}"
					}"}>{$data.star_name}の金運をチェック♬</a>
						{elseif $data.data_type == "money"}
							<a class="link_style" href="{sitelink mode="love/{$data.star_name_en}"
					}"}>{$data.star_name}の恋愛運をチェック♬</a>
						{/if}
					</div>
				</div>
				<div class="side ">
					{include file='sidebar.tpl'}
				</div>
			</div>
		{else}
			<div class="container">
				<div class="space">
					<h2 style="text-align: center;">取得した情報がありません。</h2>
				</div>
			</div>
		{/if}
{/block}