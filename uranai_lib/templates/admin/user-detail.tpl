{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}

<div class="text-right">
	{makelink mode="user" action="listing" value="一覧へ戻る" class="btn btn-warning"}
</div>

<div class="col-sm-6 col-sm-offset-3">
	<div class="well">
		<h2> <i class="fa fa-user"></i> ユーザ情報</h2>

		<div class="form-group">
			<label for="email">メールアドレス</label>
			<div class="form-control">{$data.user.email}</div>
		</div>
		<div class="form-group">
			<label for="handlename">ハンドルネーム</label>
			<div class="form-control">{$data.user.handlename}</div>
		</div>
		<div class="form-group">
			<label>性別 </label>
			<div class="form-control">
				{if $data.user.gender=='male'}男性{/if}
				{if $data.user.gender=='female'}女性{/if}
			</div>
		</div>
		<div class="form-group">
			<label for="birthday">生年月日</label>
			<div class="form-control">{$data.user.birthday}</div>
		</div>

		<h2><i class="fa fa-rss"></i> 送信設定</h2>

		<div class="form-group">
			<label>時間</label>
			<div class="form-control">
				{$data.user.notificationHour}
			</div>

			<hr>

			<label>曜日 </label>
			<div class="clearfix">
				<span class="label label-{if $data.user.notification1}success{else}danger{/if}">月曜日</span>
				<span class="label label-{if $data.user.notification2}success{else}danger{/if}">火曜日</span>
				<span class="label label-{if $data.user.notification3}success{else}danger{/if}">水曜日</span>
				<span class="label label-{if $data.user.notification4}success{else}danger{/if}">木曜日</span>
				<span class="label label-{if $data.user.notification5}success{else}danger{/if}">金曜日</span>
				<span class="label label-{if $data.user.notification6}success{else}danger{/if}">土曜日</span>
				<span class="label label-{if $data.user.notification0}success{else}danger{/if}">日曜日</span>
			</div>
			<br>

			<div class="clearfix"><span class="label label-{if $data.user.notificationHolidays}success{else}danger{/if}">祝日</span></div>
		</div>

	</div>
</div>

{/block}
