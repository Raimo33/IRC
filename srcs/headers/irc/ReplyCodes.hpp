/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReplyCodes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:57:53 by craimond          #+#    #+#             */
/*   Updated: 2024/05/22 14:36:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLYCODES_HPP
# define REPLYCODES_HPP

# define RPL_WELCOME			1
# define RPL_YOURHOST			2
# define RPL_CREATED			3
# define RPL_MYINFO				4
# define RPL_ISUPPORT			5
# define RPL_BOUNCE				10
# define RPL_MAP				15
# define RPL_MAPEND				17
# define RPL_MAPSTART			18
# define RPL_HELLO				20
# define RPL_YOURID				42
# define RPL_SAVENICK			43
# define RPL_TRACELINK			200
# define RPL_TRACECONNECTING	201
# define RPL_TRACEHANDSHAKE		202
# define RPL_TRACEUNKNOWN		203
# define RPL_TRACEOPERATOR		204
# define RPL_TRACEUSER			205
# define RPL_TRACESERVER		206
# define RPL_TRACESERVICE		207
# define RPL_TRACENEWTYPE		208
# define RPL_TRACECLASS			209
# define RPL_STATSLINKINFO		211
# define RPL_STATSCOMMANDS		212
# define RPL_STATSCLINE			213
# define RPL_STATSNLINE			214
# define RPL_STATSILINE			215
# define RPL_STATSKLINE			216
# define RPL_STATSQLINE			217
# define RPL_STATSYLINE			218
# define RPL_ENDOFSTATS			219
# define RPL_UMODEIS			221
# define RPL_SERVICEINFO		231
# define RPL_ENDOFSERVICES		232
# define RPL_SERVICE			233
# define RPL_SERVLIST			234
# define RPL_SERVLISTEND		235
# define RPL_STATSIAUTH			239
# define RPL_STATSVLINE			240
# define RPL_STATSLLINE			241
# define RPL_STATSUPTIME		242
# define RPL_STATSOLINE			243
# define RPL_STATSHLINE			244
# define RPL_STATSSLINE			245
# define RPL_STATSPING			246
# define RPL_STATSBLINE			247
# define RPL_STATSDEFINE		248
# define RPL_STATSDEBUG			249
# define RPL_STATSDLINE			250
# define RPL_LUSERCLIENT		251
# define RPL_LUSEROP			252
# define RPL_LUSERUNKNOWN		253
# define RPL_LUSERCHANNELS		254
# define RPL_LUSERME			255
# define RPL_ADMINME			256
# define RPL_ADMINLOC1			257
# define RPL_ADMINLOC2			258
# define RPL_ADMINEMAIL			259
# define RPL_TRACELOG			261
# define RPL_TRACEEND			262
# define RPL_TRYAGAIN			263
# define RPL_LOCALUSERS			265
# define RPL_GLOBALUSERS		266
# define RPL_NONE				300
# define RPL_AWAY				301
# define RPL_USERHOST			302
# define RPL_ISON				303
# define RPL_TEXT				304
# define RPL_UNAWAY				305
# define RPL_NOWAWAY			306
# define RPL_WHOISUSER			311
# define RPL_WHOISSERVER		312
# define RPL_WHOISOPERATOR		313
# define RPL_WHOWASUSER			314
# define RPL_ENDOFWHO			315
# define RPL_WHOISCHANOP		316
# define RPL_WHOISIDLE			317
# define RPL_ENDOFWHOIS			318
# define RPL_WHOISCHANNELS		319
# define RPL_LISTSTART			321
# define RPL_LIST				322
# define RPL_LISTEND			323
# define RPL_CHANNELMODEIS		324
# define RPL_UNIQOPIS			325
# define RPL_NOTOPIC			331
# define RPL_TOPIC				332
# define RPL_TOPIC_WHO_TIME		333
# define RPL_INVITING			341
# define RPL_SUMMONING			342
# define RPL_REOPLIST			344
# define RPL_ENDOFREOPLIST		345
# define RPL_INVITELIST			346
# define RPL_ENDOFINVITELIST	347
# define RPL_EXCEPTLIST			348
# define RPL_VERSION			351
# define RPL_ENDOFEXCEPTLIST	349
# define RPL_WHOREPLY			352
# define RPL_NAMREPLY			353
# define RPL_KILLDONE			361
# define RPL_CLOSING			362
# define RPL_CLOSEEND			363
# define RPL_LINKS				364
# define RPL_ENDOFLINKS			365
# define RPL_ENDOFNAMES			366
# define RPL_BANLIST			367
# define RPL_ENDOFBANLIST		368
# define RPL_ENDOFWHOWAS		369
# define RPL_INFO				371
# define RPL_MOTD				372
# define RPL_INFOSTART			373
# define RPL_ENDOFINFO			374
# define RPL_MOTDSTART			375
# define RPL_ENDOFMOTD			376
# define RPL_YOUREOPER			381
# define RPL_REHASHING			382
# define RPL_YOURESERVICE		383
# define RPL_MYPORTIS			384
# define RPL_NOTOPERANYMORE		385
# define RPL_TIME				391
# define RPL_USERSSTART			392
# define RPL_USERS				393
# define RPL_ENDOFUSERS			394
# define RPL_NOUSERS			395
# define ERR_NOSUCHNICK			401
# define ERR_NOSUCHSERVER		402
# define ERR_NOSUCHCHANNEL		403
# define ERR_CANNOTSENDTOCHAN	404
# define ERR_TOOMANYCHANNELS	405
# define ERR_WASNOSUCHNICK		406
# define ERR_TOOMANYTARGETS		407
# define ERR_NOSUCHSERVICE		408
# define ERR_NOORIGIN			409
# define ERR_NORECIPIENT		411
# define ERR_NOTEXTTOSEND		412
# define ERR_NOTOPLEVEL			413
# define ERR_WILDTOPLEVEL		414
# define ERR_BADMASK			415
# define ERR_TOOMANYMATCHES		416
# define ERR_UNKNOWNCOMMAND		421
# define ERR_NOMOTD				422
# define ERR_NOADMININFO		423
# define ERR_FILEERROR			424
# define ERR_NONICKNAMEGIVEN	431
# define ERR_ERRONEOUSNICKNAME	432
# define ERR_NICKNAMEINUSE		433
# define ERR_SERVICENAMEINUSE	434
# define ERR_SERVICECONFUSED	435
# define ERR_NICKCOLLISION		436
# define ERR_UNAVAILRESOURCE	437
# define ERR_USERNOTINCHANNEL	441
# define ERR_NOTONCHANNEL		442
# define ERR_USERONCHANNEL		443
# define ERR_NOLOGIN			444
# define ERR_SUMMONDISABLED		445
# define ERR_USERSDISABLED		446
# define ERR_NOTREGISTERED		451
# define ERR_NEEDMOREPARAMS		461
# define ERR_ALREADYREGISTRED	462
# define ERR_NOPERMFORHOST		463
# define ERR_PASSWDMISMATCH		464
# define ERR_YOUREBANNEDCREEP	465
# define ERR_YOUWILLBEBANNED	466
# define ERR_KEYSET				467
# define ERR_CHANNELISFULL		471
# define ERR_UNKNOWNMODE		472
# define ERR_INVITEONLYCHAN		473
# define ERR_BANNEDFROMCHAN		474
# define ERR_BADCHANNELKEY		475
# define ERR_BADCHANMASK		476
# define ERR_NOCHANMODES		477
# define ERR_BANLISTFULL		478
# define ERR_NOPRIVILEGES		481
# define ERR_CHANOPRIVSNEEDED	482
# define ERR_CANTKILLSERVER		483
# define ERR_RESTRICTED			484
# define ERR_UNIQOPRIVSNEEDED	485
# define ERR_NOOPERHOST			491
# define ERR_NOSERVICEHOST		492
# define ERR_STATSKLINE			499
# define ERR_UMODEUNKNOWNFLAG	501
# define ERR_USERSDONTMATCH		502
# define RPL_ETRACEFULL			708
# define RPL_ETRACEEND			759

# include <map>
# include <string>
# include <stdint.h>

namespace irc
{
	const std::map<uint16_t, std::string>			create_reply_codes(void);
	extern const std::map<uint16_t, std::string>	reply_codes;
}

#endif