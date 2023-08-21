{extends file="main.tpl"}
{block name=body}

<div class="text-right">
	{makelink mode="news" action="listing" value="一覧へ戻る" class="btn btn-warning"}
</div>
<div class="clear">&nbsp;</div>

<div class="col-sm-8 col-sm-offset-2">

	{if $data.message}
	<div class="alert alert-danger">{$data.message}</div>
	{/if}

	<form action="" class="well" method="post">
		<div class="form-group">
			<label>タイトル</label>
			<input name="news_title" class="form-control" type="text" value="{$data.news_title|escape}">
		</div>
		
		<div class="form-group">
			<label>本文</label>
			<textarea name="news_content" class="form-control" rows="10">{$data.news_content|default:"<p class='betline'>\n\n</p>"}</textarea>
		</div>

		<div class="row">
			<label>表示</label>
			<div class="col-sm-6">
				<div class="form-group">
					<label>公開日</label>
					<input name="news_release_date" class="form-control" type="text" value="{$data.news_release_date|default:$data.default_release_date}">
				</div>

				<div class="form-group">
					<label>サイドバーに表示</label>
					<input name="promote_from_date" class="form-control" type="text" value="{$data.promote_from_date|default:$data.default_release_date}">
					～
					<input name="promote_until_date" class="form-control" type="text" value="{$data.promote_until_date|default:$data.default_promote_date}">
				</div>

			</div>
			<div class="col-sm-6">
				<div class="form-group">
					<input id="is_show_yes" name="news_is_show" class="form-radio" type="radio" value="1" {if $data.news_is_show=="1"}checked{/if}>
					<label for="is_show_yes">表示する</label>
				</div>
				<div class="form-group">
					<input id="is_show_no" name="news_is_show" class="form-radio" type="radio" value="0" {if $data.news_is_show=="0"}checked{/if}>
					<label for="is_show_no">表示しない</label>
				</div>
				<p class="text-info">
					<i class="fa fa-arrow-left"></i><b>日付の形式：</b><br>
					YYYY-MM-DD<br>又は<br>YYYY-MM-DD HH:mm:ss
				</p>
			</div>
		</div>

		<hr>

		<dl class="disable">
			<dt>作成び</dt>
			<dd>{$data.date_create|default:"--"}</dd>
			<dt>更新日</dt>
			<dd>{$data.date_update|default:"--"}</dd>
		</dl>

		<hr>

		<div class="text-center">
			<input type="hidden" name="mode" value="news">
			<input type="hidden" name="action" value="update">
			<input type="hidden" name="news_id" value="{$data.news_id}">
			<button class="btn btn-success">保存</button>
		</div>
	</form>
</div>

{/block}
