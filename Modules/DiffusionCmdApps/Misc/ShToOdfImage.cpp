/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center.

All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include <mitkImageCast.h>
#include <mitkImage.h>
#include <mitkIOUtil.h>
#include "mitkDiffusionCommandLineParser.h"
#include <mitkPreferenceListReaderOptionsFunctor.h>
#include <itkShToOdfImageFilter.h>
#include <mitkShImage.h>
#include <mitkDiffusionFunctionCollection.h>

int main(int argc, char* argv[])
{
  mitkDiffusionCommandLineParser parser;

  parser.setTitle("ShToOdfImage");
  parser.setCategory("Preprocessing Tools");
  parser.setDescription("Calculate discrete ODF image from SH coefficient image");
  parser.setContributor("MIC");

  parser.setArgumentPrefix("--", "-");
  parser.addArgument("", "i", mitkDiffusionCommandLineParser::String, "Input:", "input image", us::Any(), false, false, false, mitkDiffusionCommandLineParser::Input);
  parser.addArgument("", "o", mitkDiffusionCommandLineParser::String, "Output:", "output image", us::Any(), false, false, false, mitkDiffusionCommandLineParser::Output);

  std::map<std::string, us::Any> parsedArgs = parser.parseArguments(argc, argv);
  if (parsedArgs.size()==0)
    return EXIT_FAILURE;

  // mandatory arguments
  std::string imageName = us::any_cast<std::string>(parsedArgs["i"]);
  std::string outImage = us::any_cast<std::string>(parsedArgs["o"]);

  try
  {
    mitk::PreferenceListReaderOptionsFunctor functor = mitk::PreferenceListReaderOptionsFunctor({"SH Image"}, {});
    mitk::ShImage::Pointer source = mitk::IOUtil::Load<mitk::ShImage>(imageName, &functor);
    mitk::Image::Pointer mitkImage = dynamic_cast<mitk::Image*>(source.GetPointer());
    mitk::OdfImage::Pointer out_image = mitk::convert::GetOdfFromShImage(mitkImage);
    if (out_image.IsNotNull())
      mitk::IOUtil::Save(out_image, outImage);
  }
  catch (const itk::ExceptionObject& e)
  {
    std::cout << e.what();
    return EXIT_FAILURE;
  }
  catch (std::exception& e)
  {
    std::cout << e.what();
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::cout << "ERROR!?!";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
