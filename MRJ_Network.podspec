#
# Be sure to run `pod lib lint MRJ_Network.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = 'MRJ_Network'
  s.version          = '0.1.8'
  s.summary          = '一个全新的网络请求框架，用来网络请求的'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

  s.description      = <<-DESC
TODO: Add long description of the pod here.
                       DESC

  s.homepage         = 'https://github.com/mrjlovetian/YHJNetwork'
  # s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'mrjlovetian@gmail.com' => 'mrjyuhongjiang@gmail.com' }
  s.source           = { :git => 'https://github.com/mrjlovetian/YHJNetwork.git', :tag => s.version.to_s }
  # s.social_media_url = 'https://twitter.com/<TWITTER_USERNAME>'

  s.ios.deployment_target = '7.0'
  s.tvos.deployment_target = '9.0'

  s.source_files = 'MRJ_Network/Classes/**/*'
  
  # s.resource_bundles = {
  #   'MRJ_Network' => ['MRJ_Network/Assets/*.png']
  # }

  # s.public_header_files = 'Pod/Classes/**/*.h'
  # s.frameworks = 'UIKit', 'MapKit'
  s.dependency 'AFNetworking', '~> 3.1'
end
