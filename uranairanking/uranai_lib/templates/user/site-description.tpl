{extends file="main.tpl"}
{block name=seo}
<title>12星座占い ランキング | {$site_details.site_name}</title>
<meta name="keywords" content="{$site_details.site_name},12星座占いランキング,占い,ランキング">
<meta name="description" content="{$site_details.site_name} １２星座占いランキング結果のレビューを表示しています♪会員登録でレビューが書けるので皆さんのレビューをお待ちしています！">

<!--OGP START-->
{include file="ogp.tpl" title="|i無料占いの紹介" des="{$site_details.site_name} １２星座占いランキング結果のレビューを表示しています♪会員登録でレビューが書けるので皆さんのレビューをお待ちしています！"}
<!--OGP END-->
{/block}

{block name=body}
<script>
// >>>
var commentStars;
$(document).on('ready', function() {
	commentStars = $("span[data-evaluation]");

	// updates
	updateCommentStars();
	checkHash();

	// events
	$(window).on('hashchange', checkHash);

	// 評価
	$('i', commentStars).click(clickCommentStar);

	// いいねボタン
	$('.btn-like').click(clickLike);

	// 自分のコメントを非表示（未公開）するための確認
	$('.hide-my-comment').click(hideMyComment);

	// コメントを報告
	$('.btn-comment-flag').click(function(e_) {
		e_.preventDefault();
		// set comment text
		var commentText = $(this).parents(".comment-area").first().find(".talk-text").html();
		var commentId   = $(this).parents(".comment-area").first().data("id");
		$("#comment-report-form .comment-text").html(commentText);
		// reset data on display
		$("#comment-report-form .form-control").val("");
		$("#comment-report-form [type=radio]").attr("checked", null);
		// set comment id in hidden
		$("#comment-report-form [name=commentId]").val(commentId);
		// display
		$("body").toggleClass("modal-open");
	});
	// report form modal close
	$(".comment-report-container .btn-close").click(function(e_) {
		e_.preventDefault();
		// hide
		$("body").toggleClass("modal-open");
	});
	$("#comment-report-form").submit(flagComment);

});

// stars click
// @param e_ jQuery Event
function clickCommentStar(e_) {
	e_.preventDefault();
	var evaluation = $(this).data('value');
	// console.log( evaluation );
	commentStars.data('evaluation', evaluation);
	// form data
	$('#comment-form [name=evaluation]').val(evaluation);
	// display
	updateCommentStars();
}

// comment form
function checkHash() {
	var hashtag = document.location.hash;
	// console.log( hashtag );

// コメントのフォームを表示
	if(hashtag == '#edit-my-comment') {
		$('#comment-form').show();
		$('.comment-alert .alert').hide();
	}

	// 私のコメントを見る
	if(/#comment-\d+/.test(hashtag)) {
		flash( $(hashtag) );
	};
}

// comment evaluation (stars)
function updateCommentStars() {
	var evaluation = commentStars.data('evaluation');
	// console.log( "---" );
	// console.log( evaluation );
	// console.log( "---" );

	var star;
	for(var i=1; i<=5; ++i) {
		star = $('i:nth-child('+i+')', commentStars);

		// clear class
		star.removeClass();
		if(i<=evaluation) {
			// checked star
			star.addClass("fa fa-star");
		}
		else {
			// blank star
			star.addClass("fa fa-star-o");
		}
	}
}

// ハートボタンをクリックする
function clickLike() {
	var comm_id = $(this).data('comment-id');
	// console.log( comm_id );

	// send to current page a post signal
	var postData = {
		likeComment : comm_id
	};

	var link = this; // link clicked (jquery object)

	// sending request
	$.post('', postData)
	.done(function(data_, status, xhr) {
		// update the counter
		$(link).next(".heart-count").text(data_.likes_count);

		// user likes? set in color
		if(data_.likes == true) {
			$(link).addClass("liked");
		}
		else {
			$(link).removeClass("liked");
		}
	})
	.fail(function(data_, status, xhr) {
		console.log( "FAULT");
	});
}


function hideMyComment(e_) {
	if(!confirm('コメントを非表示にしますか？')) {
		e_.preventDefault();
		return false;
	}
	else {
		return true;
	}
}

// コメントの違反報告アクション
// this = comment report form
function flagComment(e_) {
	e_.preventDefault();

	var form = this;

	// send to current page a post signal
	var postData = {
		reportComment: $("[name=commentId]", this).val(),
		violationCategory: $("[name=violationCategory]:checked", this).val(),
		violationComment: $("[name=violationComment]", this).val(),
		reporterName: $("[name=reporterName]", this).val(),
		reporterCompany: $("[name=reporterCompany]", this).val(),
		reporterEmail: $("[name=reporterEmail]", this).val()
	};

	var flagSuccess = function(msg_) {
		$('body').removeClass('modal-open');
		alert(msg_);
	};

	var flagError = function(msg_) {
		alert(msg_);
	}

	// sending request
	$.post('', postData)
	.done(function(data_, status, xhr) {
		if(data_.status=='OK') {
			flagSuccess(data_.message);
		}
		else {
			flagSuccess(data_.message);
		}
	})
	.fail(function(data_, status, xhr) {
		flagError("報告できませんでした。");
	});
}
// <<<
</script>

<div class="container site-description">
	<div class="title row text-center">
		<h2 class="font-color" title="{$site_details.site_name}">{$site_details.site_name} について</h2>
	</div>

	<div class="spadding-top">
		<div style="color:black;" class="base-bg contents-space clearfix">
			
			<a class="btn-external-sitelink btn-lg button" href='{$site_details|siteLinkDecide}' target="_blank" title="{$site_details.site_name}">『{$site_details.site_name}』を開く</a>

			<div class="row comprehensive-evaluation">
				<div class="col-sm-4">
					{insert siteStarAverage comments=$site_evaluations}
				</div>
				<div class="col-sm-8">
					評価別コメント数<br>
					{insert siteStarDetails comments=$site_evaluations}
				</div>

			</div>

			<!-- ads -->
			<div class="contents-space-negative col-lg-12 adg visible-xs visible-sm visible-md visible-lg">
				<div class="ad-bg">
				{if $config.browser_name == "IE" || $config.browser_name == "Edge"}
				{insert ad_group id="6"}
				{else}
				{insert ad_group id="4"}
				{/if}
				</div>
			</div><!-- ads end -->


			<div class="row desc-content-area">
				{if $site_details.visible}
				<div class="col-sm-9 desc-fukidasi">
					
					<blockquote cite="{$site_details|siteLinkDecide}">
					<div class="section" title="{$site_details.site_name}">{$site_details.site_name}サイトの紹介</div>
					<p class="site-description">
						{$site_details.description}
						
					</p>
					<div class="quote">
						<a href='{$site_details|siteLinkDecide}' target="_blank" title="{$site_details.site_name}">『{$site_details.site_name}』より引用</a>
					</div>
					</blockquote>
					<div class="section">サイトの感想・おすすめポイント</div>
					<div class="site-presentation">
						{$site_details.presentation}
					</div>
				</div>
				<div class="col-sm-3 desc-character">
					<img src="/user/img_re/n-{$img_charcter}.png" alt="" class="width-max">
				</div>
				{/if}
			</div>

			<div class="comment-box">

				<div class="comment-container">

					{assign "form_visible" "none"}
					<div class="comment-alert">
						{if !$user}
							{* not logged in *}
							<div class="alert alert-info">
								コメント機能を使用するためには、ログインが必要です。<br>
								<a class="alert-link" href="{sitelink mode='account/login'}">ログインはこちら</a>
							</div>
						{elseif !$post_status && $user_site_comment && $user_site_comment.status == 'published'}
							{* change comment? *}
							<div class="alert alert-info">
								このサイトのコメントは投稿されております。<br>
								<a class="alert-link see-my-comment" href="{sitelink mode="site-description/{$site_details.site_id}/page{$user_site_comment_page}"}#comment-{$user_site_comment.site_comment_id}">自分のコメントを見る</a>
							</div>
						{elseif !$post_status && $user_site_comment && $user_site_comment.status == 'rejected'}
							{* rejected comment? *}
							<div class="alert alert-warning">
								いただいたコメントは審査に通りませんでした。<br>
								<a class="alert-link see-my-comment" href="#edit-my-comment" title="コメントを直す">コメントを直す</a>
							</div>
						{elseif $post_status}
							{* フォームデータ *}
							{if $post_status=='pending'}
								<div class="alert alert-info">
									評価していただき、ありがとうございました。<br>
									コメントのレビューに入らせていただきます。
								</div>
							{elseif $post_status=='published'}
								<div class="alert alert-info">
									評価していただき、ありがとうございました。
								</div>
							{elseif $post_status=='error_already_pending'}
								<div class="alert alert-danger">
									評価を投稿できません。 <br>
									現在レビュー中です。
								</div>
							{elseif $post_status=='error_impossible'}
								<div class="alert alert-danger">
									エラー、データの保存ができませんでした。<br>
									しばらくたってから、もう一度登録してください。
								</div>
							{/if}
						{elseif $user_site_comment && $user_site_comment.status == 'pending'}
							{* リビュー途中 *}
							<div class="alert alert-info">
								このサイトのコメントは投稿されております。<br>
								現在レビュー中ですので、反映までしばらくお待ちください。
							</div>
						{elseif $user_site_comment && $user_site_comment.status == 'hidden'}
							{* 未公開の場合は *}
							<div class="alert alert-info">
								すでに、コメントは投稿済です。<br>
								コメントが未公開になっています。<br>
								<a class="alert-link" href="{sitelink mode='account/comment'}#comment-{$user_site_comment.site_comment_id}">マイページで見る</a>
							</div>
						{else}
							{assign "form_visible" "block"}
						{/if}
					</div>

					<a name="edit-my-comment"></a>
					<form id="comment-form" action="#post-comment" method="post" style="display: {$form_visible}">
						<input type="hidden" name="mode" value="site-description">
						<input type="hidden" name="action" value="post-site-comment">
						
						{* 無効コメントのため *}
						<input type="hidden" name="current_id" value="{$user_site_comment.site_comment_id}">
						<input type="hidden" name="current_status" value="{$user_site_comment.status}">
						<input type="hidden" name="current_parent" value="{$user_site_comment.parent_revision}">

						{* 新規・公開中コメントのため *}
						<input type="hidden" name="parent_revision" value="{$user_site_comment.site_comment_id|default:0}">
						<input type="hidden" name="evaluation" value="{$user_site_comment.evaluation|default:5}">


						<div class="input-form">
							<div class="input-title" title="{$site_details.site_name}">{$site_details.site_name}についての口コミを書く</div>
							<p class="tecen">
								不適切な内容が無いか確認させていただいているため、
								<br>コメントを投稿・編集していただいてから、反映まで1～3営業日かかります。ご了承ください。
								<br>なお、コメント投稿時にハンドルネームが公開されます。ハンドルネームの変更は<a href="{sitelink mode="account/form"}">こちら</a>。
							</p>
							{if $rejected[$user_site_comment.site_comment_id]}
							{include "site-comment-rejected.part.tpl" rejected=$rejected[$user_site_comment.site_comment_id]}
							{/if}

							<table>
								<tr>
									<th>評価</th>
									<td>
										<span class="star" data-evaluation="{$user_site_comment.evaluation|default:5}">
											<i class="fa fa-star-o" data-value="1"></i>
											<i class="fa fa-star-o" data-value="2"></i>
											<i class="fa fa-star-o" data-value="3"></i>
											<i class="fa fa-star-o" data-value="4"></i>
											<i class="fa fa-star-o" data-value="5"></i>
										</span>
									</td>
								</tr>
								<tr>
									<th>コメント</th>
									<td>
										<textarea name="comment" class="form-control">{$user_site_comment.comment}</textarea>
									</td>
								</tr>
							</table>
							<div class="tecen">
								<input type="submit" class="btn btn-primary" value="送信">
							</div>
						</div>
					</form>

				</div>

				{* 一覧 *}
				<a name="comments-list"></a>
				<div class="comment-container">
					{if $comments != null}<h3 title="{$site_details.site_name}">{$site_details.site_name}の評価はこちら</h3>{/if}
					{foreach $comments as $comment}
					<a name="comment-{$comment.site_comment_id}"></a>
					<div id="comment-{$comment.site_comment_id}" data-id="{$comment.site_comment_id}" class="comment-area clearfix {$comment.status}">
						{if $comment.comment}
						<div class="comment-flag-container">
							<a class="btn btn-comment-flag" href="#flag">
								<i class="fa fa-flag"></i>
							</a>
						</div>
						{/if}

						<div class="table-cell icon-img">
							{insert userAvatar avatar=$comment.user_avatar}
						</div>
						
						<div class="talk table-cell">
							<div class="nickname" data-id="{$comment.user_id}">{$comment.handlename}さん</div>
							<div class="date">{$comment.date_create|japanesedate}</div>
							<div class="hyouka">{insert siteEvaluationStars evaluation=$comment.evaluation}　</div>
							<div class="talk-text">{$comment.comment|nl2br}</div>

							<div class="comment-bottom">
								{if $user.user_id == $comment.user_id && $user_site_comment.parent_revision != $comment.site_comment_id}
								<span class="function-icon"><a href="#edit-my-comment" title="コメントを変更"><i class="fa fa-pencil"></i></a></span>
								{/if}

								{if $user.user_id == $comment.user_id}
								<span class="function-icon"><a href='{sitelink mode="site-description/{$comment.site_id}/hide/{$comment.site_comment_id}"}' title="コメントを未公開" class="hide-my-comment"><i class="fa fa-eye-slash"></i></a></span>
								{/if}

								<span class="function-icon">
									{if $user.user_id}
									<a href="#like" class="btn-like {if $comment.user_likes}liked{/if}" data-comment-id="{$comment.site_comment_id}"><i class="fa fa-heart"></i></a>
									{else}
									<i class="fa fa-heart"></i>
									{/if}
									<span class="heart-count">{$comment.likes_count}</span>
								</span>
							</div>
						</div>
					</div>
					{/foreach}

					<!-- <script src='/user/js/paginator.js'></script> -->
					{assign link {sitelink mode="site-description/{$site_details.site_id}/page[PAGE]"}}
					{insert paginator page_name='site-comments' format="$link#comments-list"}
				</div>

			</div>

			<!-- ads -->
			<div style="margin-bottom:20px;" class="contents-space-negative col-lg-12 adg visible-xs visible-sm visible-md visible-lg">
				<div class="ad-bg">
				{if $config.browser_name == "IE" || $config.browser_name == "Edge"}
				{insert ad_group id="6"}
				{else}
				{insert ad_group id="4"}
				{/if}
				</div>
			</div>
			<hr class="ad_sep">

			<!-- ads end -->

			<a class="btn-external-sitelink btn-lg button" href='{$site_details|siteLinkDecide}' target="_blank" title="{$site_details.site_name}">『{$site_details.site_name}』を開く</a>

		</div>
	</div>	
</div>

{* コメントの違反報告モダール画面 *}

<div id="comment-report-modal" class="modal-filter">
	<div class="comment-report-container">
		<a class="btn btn-close" href="#close"><i class="fa fa-times"></i> 閉じる</a>

		<h2 class="font-color tecen"><span class="word-break">違反報告フォーム</h2>

		<div class="comment-report-form-scroller">
			<div class="base-bg base-format">
				<h3 class="tecen">お問い合わせをする前に、以下の点をご了承ください</h3>
				<ul class="list-padding">
					<li>頂いた違反報告に対しての回答は行っておりません。</li>
					<li>頂いた報告をもとに、情報を確認する作業はお時間を頂くことがあり、即時削除等の対応ができない場合がございます</li>
					<li>「私はそうは思わないから」「内容に腹が立つから」等の、あいまいな理由の場合は、対応いたしかねますのでご了承ください。</li>
					<li>「<span class="warning">※</span>」のついているものは必ず入力してください</li>
				</ul>
				<form id="comment-report-form">
					<input type="hidden" name="commentId" value="">

					<div class="form-group">
						<label>報告するコメント</label>
						<blockquote class="comment-text">コメント内容</blockquote>
					</div>
					<div class="form-group">
						<label>報告理由<span class="warning">※</span></label>
						{foreach $violations as $v}
						<div>
							<label><input type="radio" name="violationCategory" value="{$v@key}" required="required"> {$v}</label>
						</div>
						{/foreach}
					</div>
					<div class="form-group">
						<label>詳細理由<span class="warning">※</span></label>
						<div><textarea name="violationComment" rows="5" class="width-max form-control" required="required"></textarea></div>
					</div>
					<div class="form-group table">
						<div><b>連絡先</b></div>
						<div class="table-row">
							<div class="table-cell">氏名</div>
							<div class="table-cell"><input type="text" name="reporterName" class="form-control"></div>
						</div>
						<div class="table-row">
							<div class="table-cell">会社名</div>
							<div class="table-cell"><input type="text" name="reporterCompany" class="form-control"></div>
						</div>
						<div class="table-row">
							<div class="table-cell">メールアドレス<span class="warning">※</span></div>
							<div class="table-cell"><input type="email" name="reporterEmail" class="form-control" required="required"></div>
						</div>
					</div>

					<p class="text-center">記載いただいた内容を送信します。よろしければ、以下のボタンをクリックしてください。</p>
					<div class="text-center"><button tyle="submit" class="btn btn-important">内容を送信する</button></div><br><br>

				</form>

			</div>
		</div>

	</div>
</div>
{/block}
