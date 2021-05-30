//
// Created by Raimundas Sakalauskas on 24/10/2018.
//

#import <Foundation/Foundation.h>

@interface ParticlePricingPlanInfo : NSObject

/*!
 * Max number of free mesh network devices available for this pricing plan.
 */
@property (nonatomic) int freeDeviceMaxCount;

/*!
 * Number of free mesh network devices currently available for this pricing plan.
 */
@property (nonatomic) int freeDevicesAvailableCount;

/*!
 * Max number of free Wifi/Ethernet mesh networks available per account.
 */
@property (nonatomic) int freeWifiNetworkMaxCount;

/*!
 * Number of free Wifi/Ethernet mesh networks currently available for this account.
 */
@property (nonatomic) int freeWifiNetworksAvailableCount;

/*!
 * Number of mesh network nodes included in current pricing plan.
 */
@property (nonatomic) int includedNodeCount;

/*
 * Number of gateway nodes included in current pricing plan.
 */
@property (nonatomic) int includedGatewayCount;

/*!
 * Number of free data in MB included in current pricing plan.
 */
@property (nonatomic) int includedDataMb;

/*!
 * Number of free months included in current pricing plan.
 */
@property (nonatomic) int freeMonths;

/*!
 * Base price of current pricing plan.
 */
@property (strong, nonatomic, nullable) NSDecimalNumber *monthlyBaseAmount;

/*!
 * Price of each MB above free data allowance.
 */
@property (strong, nonatomic, nullable) NSDecimalNumber *overageMinCostMb;

- (instancetype)initWithParams:(NSDictionary *)params;

@end

@interface ParticlePricingInfo : NSObject

/*!
 * Whether current plan is available for the user.
 */
@property (nonatomic) BOOL allowed;

/*!
 * Whether current plan is free or chargeable for the user.
 */
@property (nonatomic) BOOL chargeable;

/*!
 * Whether user has credit card on file.
 */
@property (nonatomic) BOOL ccOnFile;

/*!
 * Last 4 digits of credit card associated with user account.
 */
@property (strong, nonatomic, nullable) NSString *ccLast4;

/*!
 * Internal name of the pricing plan.
 */
@property (strong, nonatomic, nullable) NSString *planSlug;

/*!
 * Detailed plan info including pricing, number of devices, gateways and data usage allowance.
 */
@property (strong, nonatomic, nonnull) ParticlePricingPlanInfo *plan;

/*!
 * Whether user will be prompted to upgrade the pricing plan.
 */
@property (nonatomic) BOOL planUpgradeNeeded;

- (instancetype)initWithParams:(NSDictionary *)params;

@end



