{extends file="main.tpl"}
{block name=seo}
<title>コメント管理 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp,ユーザー情報">
<meta name="description" content="12星座占いサイトを独自に集計しランキングを出しています。ユーザー情報の登録、変更ページです。">
{/block}
{block name=body}
<link rel="stylesheet" href="/formCheck/css/formCheck.css">
<script>
$(document).on('ready', function() {
		var hashtag = document.location.hash;
		flash( $(hashtag) );
});
</script>

<div class="container user-comment">
	<div class="page-center clearfix">
		<div class="col-sm-10 col-sm-offset-1">

			<p>
			{if $user}
				<a href="{sitelink mode="mypage"}" class="btn btn-primary"><i class="fa fa-arrow-left"></i> マイページへ戻る</a>
			{/if}
			</p>

			{if $message}
				<div class="alert alert-{$message.status}">{$message.content}</div>
			{/if}
			
			<div class="base-bg contents-space clearfix">
			{if $user}
				<p class="tecen">
					不適切な内容が無いか確認させていただいているため、
					<br>コメントを編集していただいた場合には、反映まで1～3営業日かかります。ご了承ください。
				</p>

				{foreach $comments as $comment}
				{include file="account-comment.part.tpl" comment=$comment rejected=$rejected}
				
				{if $revisions[{$comment.parent_revision}]}
					<div class="comment-review-container">
						{include file="account-comment.part.tpl" comment=$revisions[$comment.parent_revision] is_review=true}
					</div>
				{/if}
				{/foreach}
			{else}
				<p class="tecen">コメントを確認するにはログインが必要です。</p>
				<div class="tecen"><a href="{sitelink mode="account/login"}" class="btn btn-primary">ログインする</a></div>
			{/if}
			</div>
		</div>
	</div>
</div>

{/block}
